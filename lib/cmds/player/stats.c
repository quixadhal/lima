/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stats.c -- print out player stats
**
//### for now, these are numeric... eventually, we'll move to a system
//### where they are "fuzzy" and use words
//### 
//### Um, that sounds nifty, but players hate it worse than death itself ..
//### -Beek
//-- If it is done WELL (and it will be .. be assured of THAT :)
//-- it's fairly painless ... problems arise when you have a set of terms 
//-- that are confusing - the prime example was ROM2's "mean" ranking
//-- which either meant poor, average, or rather good <grin> noone
//-- except a couple of people were sure, and they weren't giving out
//-- info. And the advantages are overwhelming: By making things a series
//-- of degrees, with enough underlying granularity, you put a damper on
//-- stat hunters/munchkins.
//-- --OH.
**
** 950813, Deathblade: Created.
*/

#include <mudlib.h>
#include <config.h>

#define SEP_MAJOR	(repeat_string("-=", 38) + "-\n")
#define SEP_MINOR	(repeat_string("-", 77) + "\n")

inherit CMD;

private void main(string arg)
{
    string name;
    string e_info = 0;
    string * guilds;
    string g_info = 0;
    string r_info = "";
    string x_info = "";
    string l_info = "";
    string o_info;
    string *curr;
    int i;

    out(SEP_MAJOR);

#ifdef USE_TITLES
    name = this_body()->query_title();
#else
    name = this_body()->query_name();
#endif
    outf("%s  (%s)\n", name, wizardp(this_user()) ? "Wizard" : "Mortal");

    out(SEP_MAJOR);

#ifdef USE_SKILLS
    e_info = sprintf("Eval: %d%%", this_body()->query_evaluation());
#endif

#ifdef USE_GUILDS
    guilds = this_body()->guilds_belong();
    if ( guilds )
    {
	if ( sizeof(guilds) > 1 )
	    g_info = "Guilds: " + implode(guilds, ", ");
	else if ( sizeof(guilds) == 0 )
	    g_info = "Guilds: none";
	else
	    g_info = "Guild: " + guilds[0];
    }
#endif

    if ( e_info && g_info )
	outf("%s  %s\n", e_info, g_info);
    else if ( e_info )
	out(e_info + "\n");
    else if ( g_info )
	out(g_info + "\n");

    out(SEP_MINOR);

#ifdef USE_RACES
    r_info = "  Race: " + capitalize(this_body()->query_race());
#endif

#ifdef USE_SIMPLE_EXP
    x_info = "  Exp: "+this_body()->query_exp();
#endif

#ifdef USE_SIMPLE_LEVEL
    l_info = "  Level: "+this_body()->query_level();
#endif

    outf("Hp: %d (%d)   Sp: %d (%d) %s%s%s\n",
	 this_body()->query_hp(), this_body()->query_max_hp(),
	 1, 1,
         r_info,l_info,x_info);

#ifdef USE_STATS
    this_body()->refresh_stats();

//### show pure stats here, too?
    outf("Basic:   Str: %-3d   Agi: %-3d  Int: %-3d  Wil: %-3d\n",
	   this_body()->query_str(), this_body()->query_agi(),
	   this_body()->query_int(), this_body()->query_wil());
    outf("Derived: Con: %-3d   Wis: %-3d  Cha: %-3d\n",
	   this_body()->query_con(), this_body()->query_wis(),
	   this_body()->query_cha());
#endif

    out(SEP_MINOR);

    o_info = "";
    curr = this_body()->query_currencies();
    if (!sizeof(curr)) {
      o_info += "You are broke.";
    }
    else {
      for (i = 0; i < sizeof(curr); i++) {
        o_info += sprintf("%s: %d  ",
          capitalize(curr[i]),this_body()->query_amt_money(curr[i]));
      }
    }
    outf("%s\n",o_info);
//### other misc stats (e.g. sober, poison, wimpy, etc)
    out("<< other misc data: sober, poison, cash, etc >>\n");

    out(SEP_MAJOR);
}
