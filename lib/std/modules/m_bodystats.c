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
#include <hooks.h>

private inherit CLASS_STATMODS;

mixed call_hooks(string, int);

private int	stat_str;	/* strength */
private int	stat_agi;	/* agility */
private int	stat_int;	/* intelligence */
private int	stat_wil;	/* willpower */

private int	stat_con;	/* constitution */
private int	stat_wis;	/* wisdom */
private int	stat_cha;	/* charisma */

/* Because of the complexity of the system, a bonus to strength affects
 * str as well as con and wis and cha.  To avoid having to recall all
 * the hooks for all the base stats every time one of the derived stats
 * is queried, we cache the bonus here, reducing the number of hook
 * calls from O(N^2) in stats to O(N).  One should make sure refresh_stats()
 * gets called occasionally; it gets called automatically once a round
 * during combat, but if you are using stats in a non-combat setting you
 * might want to manually call it to make sure the latest modifications
 * are being used (some of the hooks might time out and remove themselves,
 * etc, but haven't done so yet b/c they haven't been checked in a while)
 */
private int	cur_str;
private int	cur_agi;
private int	cur_int;
private int	cur_wil;
private int	cur_con;
private int	cur_wis;
private int	cur_cha;

// This is defined in such a way that:
//
// derived_stat[i] = trans_matrix[i][j] * base_stat[j]
//                   + constant_vector[i] + skill contribution
//
// where there is an implied sum over j
//
nosave private mixed *trans_matrix;
nosave private int *constant_vector;

#define DEFAULT_RANGE	6
#define BASE_VALUE	20

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
    return cur_str;
}

nomask int query_agi()
{
    return cur_agi;
}
nomask int query_int()
{
    return cur_int;
}

nomask int query_wil()
{
    return cur_wil;
}

#define CON 0
#define WIS 1
#define CHA 2

#define STR 0
#define AGI 1
#define INT 2
#define WIL 3

/*
** Derived statistics
*/
//:FUNCTION refresh_stats
//refresh_stats() recalculates all the stats and requery's all the bonuses.
//Combat calls this once a round.  If you are using stats in a non-combat
//setting, you might want to call this first.
void refresh_stats() {
//:HOOK str_bonus
//Used to modify strength
//:HOOK agi_bonus
//Used to modify agility
//:HOOK int_bonus
//Used to modify intelligence
//:HOOK wil_bonus
//Used to modify willpower
    int adj_str, adj_agi, adj_int, adj_wil;

    cur_str = stat_str + (adj_str = call_hooks("str_bonus", HOOK_SUM));
    cur_agi = stat_agi + (adj_agi = call_hooks("agi_bonus", HOOK_SUM));
    cur_int = stat_int + (adj_int = call_hooks("int_bonus", HOOK_SUM));
    cur_wil = stat_wil + (adj_wil = call_hooks("wil_bonus", HOOK_SUM));

    // optimize common case
    if (!(adj_str || adj_agi || adj_int || adj_wil)) {
	cur_con = stat_con;
	cur_wis = stat_wis;
	cur_cha = stat_cha;
	return;
    }

    cur_con = stat_con + (trans_matrix[CON][STR] * adj_str +
	trans_matrix[CON][AGI] * adj_agi +
	trans_matrix[CON][INT] * adj_int +
	trans_matrix[CON][WIL] * adj_wil + 5000) / 10000;
    cur_wis = stat_wis + (trans_matrix[WIS][STR] * adj_str +
	trans_matrix[WIS][AGI] * adj_agi +
	trans_matrix[WIS][INT] * adj_int +
	trans_matrix[WIS][WIL] * adj_wil + 5000) / 10000;
    cur_cha = stat_cha + (trans_matrix[CHA][STR] * adj_str +
	trans_matrix[CHA][AGI] * adj_agi +
	trans_matrix[CHA][INT] * adj_int +
	trans_matrix[CHA][WIL] * adj_wil + 5000) / 10000;
}

void recompute_derived() {
    stat_con = (constant_vector[CON] * 10000 + 
	trans_matrix[CON][STR] * stat_str +
	trans_matrix[CON][AGI] * stat_agi +
	trans_matrix[CON][INT] * stat_int +
	trans_matrix[CON][WIL] * stat_wil + 5000) / 10000;
    stat_wis = (constant_vector[WIS] * 10000 +
	trans_matrix[WIS][STR] * stat_str +
	trans_matrix[WIS][AGI] * stat_agi +
	trans_matrix[WIS][INT] * stat_int +
	trans_matrix[WIS][WIL] * stat_wil + 5000) / 10000;
    stat_cha = (constant_vector[CHA] * 10000 +
	trans_matrix[CHA][STR] * stat_str +
	trans_matrix[CHA][AGI] * stat_agi +
	trans_matrix[CHA][INT] * stat_int +
	trans_matrix[CHA][WIL] * stat_wil + 5000) / 10000;
    refresh_stats();
}

//### these are also in race.c
#define WIS_SKILL_FACTOR	30
#define CHA_SKILL_FACTOR	25

//### these should come from skills
int skill_wis_sum() {
    return 0;
}

int skill_cha_sum() {
    return 0;
}

nomask int query_con_pure() {
    return stat_con;
}

nomask int query_wis_pure() {
    return stat_wis + (WIS_SKILL_FACTOR * skill_wis_sum()/100);
}

nomask int query_cha_pure() {
    return stat_cha + (CHA_SKILL_FACTOR * skill_cha_sum()/100);
}

nomask int query_con() {
    return cur_con;
}

nomask int query_wis() {
    return cur_wis + (WIS_SKILL_FACTOR * skill_wis_sum()/100);
}

nomask int query_cha() {
    return cur_cha + (CHA_SKILL_FACTOR * skill_cha_sum()/100);
}

/*
** Character creation stuff
**
** query_roll_mods() may be overriden by subclasses to provide for changes
**   in the statistics generation.
**
** init_stats() may only be called once (admins may call whenever)
*/
class stat_roll_mods query_roll_mods()
{
    // should be overloaded
    return new(class stat_roll_mods);
}

mixed *query_transformation_matrix()
{
    // should be overloaded
    return ({ allocate(4), allocate(4), allocate(4) });
}

int *query_constant_vector() {
    // should be overloaded
    return allocate(3);
}

private nomask int roll_stat(int adjust, int range)
{
    if ( !range )
	range = DEFAULT_RANGE;

    return BASE_VALUE + adjust + random(range) - (range + 1)/2;
}

//:FUNCTION init_stats
//Rolls the stats for the first time, based on the proper racial adjustments.
//Admins can call this to reinitialize a player's stats (for example, in the
//case of abysmally horrific (near minimum) rolls.
nomask void init_stats()
{
    class stat_roll_mods mods;

    if ( stat_str && !check_previous_privilege(1) )
	error("cannot reinitialize statistics\n");

    mods = query_roll_mods();
    if ( mods->str_adjust + mods->agi_adjust + mods->int_adjust +
	 mods->wil_adjust != 0 )
	error("illegal stat adjustment values\n");

    stat_str = roll_stat(mods->str_adjust, mods->str_range);
    stat_agi = roll_stat(mods->agi_adjust, mods->agi_range);
    stat_int = roll_stat(mods->int_adjust, mods->int_range);
    stat_wil = roll_stat(mods->wil_adjust, mods->wil_range);

    recompute_derived();
    refresh_stats();
}

/*
** Object inheriting this module should be sure to call this.
*/
void internal_setup(mixed args...)
{
    trans_matrix = query_transformation_matrix();
    constant_vector = query_constant_vector();
}
