/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** bodystats.c -- body statistics (characteristics)
**
** This module should be inherited into anything requiring physical
** statistics/characteristics.  Typically, this will be used by a
** player's body, or an NPC.
**
** Stat values have the range: [0,100].  Typical values:
**
**	10	Joe Human
**	20	Newbie Hero (average value at character creation time)
**	50	Mid-level experience
**	80	Master/high-level
**	90	History will remember you
**	95	Frightening
**	100	Pure god-like status
**
** Statistics have a number of basic values which are "rolled" at
** character creation time.  These differentiate starting characters
** from one another.
**
**	Strength:	physical stature, power, brute force
**		Weakling vs. strong/powerful
**
**	Agility:	body control, speed, flexibility
**		Uncoordinated vs. excellent control
**
**	Intelligence:	inherent capability for learning
**		Dumb vs. smart/capable
**
**	Willpower:	drive, stamina
**		Unmotivated vs. driven/fanatical
**
** A good number of statistics are derived from the base statistics.
**
**	Constitution:	physical/mental stamina, sturdiness
**		Elf vs. dwarf
**
**	Wisdom:		collected knowledge
**		Naive vs. wise
**
**	Charisma:	natural attraction, leadership, persuasion
**		Nobody vs. great leader/speaker
**
** 950813, Deathblade: created.
*/

#include <classes.h>

private inherit CLASS_STATMODS;

private int	stat_str;	/* strength */
private int	stat_agi;	/* agility */
private int	stat_int;	/* intelligence */
private int	stat_wil;	/* willpower */

static private int adj_str;	/* adjustment for str */
static private int adj_agi;	/* adjustment for agi */
static private int adj_int;	/* adjustment for int */
static private int adj_wil;	/* adjustment for wil */

static private class stat_extra_mods extra_mods;

#define DEFAULT_RANGE	6
#define BASE_VALUE	20


/*
** Effects of the base stats on the derived stats
*/
#define CON_STR_FACTOR		15
#define CON_AGI_FACTOR		15
#define CON_WIL_FACTOR		40
#define CON_RACE_FACTOR		30

#define WIS_STR_FACTOR		5
#define WIS_AGI_FACTOR		5
#define WIS_INT_FACTOR		25
#define WIS_WIL_FACTOR		15
#define WIS_RACE_FACTOR		25
#define WIS_SKILL_FACTOR	25

#define CHA_STR_FACTOR		10
#define CHA_AGI_FACTOR		10
#define CHA_INT_FACTOR		20
#define CHA_WIL_FACTOR		20
#define CHA_WIS_FACTOR		20	/* note: derived */
#define CHA_SKILL_FACTOR	20
/* plus self/target race modifier */


/*
** Base statistics
**
** query_xxx_pure() returns the "pure" unadulterated statistic.  These
**   values are typically never used.
**
** query_xxx() returns the statistic, adjusted for all additional factors
**   such as temporary boosts, detriments, etc.
*/
nomask int query_str_pure()
{
    return stat_str;
}
nomask int query_agi_pure()
{
    return stat_agi;
}
nomask int query_int_pure()
{
    return stat_int;
}
nomask int query_wil_pure()
{
    return stat_wil;
}

nomask int query_str()
{
    return stat_str + adj_str;
}
nomask int query_agi()
{
    return stat_agi + adj_agi;
}
nomask int query_int()
{
    return stat_int + adj_int;
}
nomask int query_wil()
{
    return stat_wil + adj_wil;
}

/*
** Derived statistics
*/
nomask int query_con()
{
    int con;
    int bonus;
    
    con = ( (stat_str + adj_str) * CON_STR_FACTOR +
	    (stat_agi + adj_agi) * CON_AGI_FACTOR +
	    (stat_wil + adj_wil) * CON_WIL_FACTOR );

    bonus = ( con
	      - (con * extra_mods->con_bonus + 50) / 100
	      + extra_mods->con_bonus ) * CON_RACE_FACTOR;

    return (con * 100 + bonus + 5000) / 10000;
}
nomask int query_wis()
{
    int wis;
    int bonus;

    wis = ( (stat_str + adj_str) * WIS_STR_FACTOR +
	    (stat_agi + adj_agi) * WIS_AGI_FACTOR +
	    (stat_int + adj_int) * WIS_INT_FACTOR +
	    (stat_wil + adj_wil) * WIS_WIL_FACTOR );

    bonus = ( wis
	      - (wis * extra_mods->wis_bonus + 50) / 100
	      + extra_mods->wis_bonus ) * WIS_RACE_FACTOR;

    /* add in portion of Eval */

    return (wis * 100 + bonus + 5000) / 10000;
}
nomask int query_cha()
{
    int wis = query_wis();

    /* add in portion of Eval */
    /* add in self/target race compatibility modifier */

    return ( (stat_str + adj_str) * CHA_STR_FACTOR +
	     (stat_agi + adj_agi) * CHA_AGI_FACTOR +
	     (stat_int + adj_int) * CHA_INT_FACTOR +
	     (stat_wil + adj_wil) * CHA_WIL_FACTOR +
	     wis * CHA_WIS_FACTOR +
	     50 ) / 100;
}

/*
** Character creation stuff
**
** query_roll_mods() may be overriden by subclasses to provide for changes
**   in the statistics generation.
**
** query_extra_mods() may be overriden to provide bonus advancements in
**   the statistics.
**
** init_stats() may only be called once (admins may call whenever)
*/
class stat_roll_mods query_roll_mods()
{
    return new(class stat_roll_mods);
}
class stat_extra_mods query_extra_mods()
{
    return new(class stat_extra_mods);
}

private nomask int roll_stat(int adjust, int range)
{
    if ( !range )
	range = DEFAULT_RANGE;

    return BASE_VALUE + adjust + random(range) - (range + 1)/2;
}

nomask void init_stats()
{
    class stat_roll_mods mods;

    if ( stat_str && !check_previous_privilege(1) )
	error("* cannot reinitialize statistics\n");

    mods = query_roll_mods();
    if ( mods->str_adjust + mods->agi_adjust + mods->int_adjust +
	 mods->wil_adjust != 0 )
	error("*illegal stat adjustment values\n");

    stat_str = roll_stat(mods->str_adjust, mods->str_range);
    stat_agi = roll_stat(mods->agi_adjust, mods->agi_range);
    stat_int = roll_stat(mods->int_adjust, mods->int_range);
    stat_wil = roll_stat(mods->wil_adjust, mods->wil_range);
}


/*
** Object inheriting this module should be sure to call this.
*/
void create()
{
    extra_mods = query_extra_mods();
}
