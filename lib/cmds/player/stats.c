/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stats.c -- print out player stats
**
//### for now, these are numeric... eventually, we'll move to a system
//### where they are "fuzzy" and use words
//### 
//### Um, that sounds nifty, but players hate it worse than death itself ..
//### -Beek
**
** 950813, Deathblade: Created.
*/

#include <mudlib.h>
#include <config.h>

inherit CMD;

private void main(string arg)
{
#ifndef USE_STATS
    out(mud_name() + " has no player statistics, sorry.\n");
#else

    string name;
    string * guilds;
    string g_info;

    outf("%76'-='s-\n", "");

#ifdef USE_TITLES
    name = this_body()->query_title();
#else
    name = this_body()->query_name();
#endif
    outf("%s  (%s)\n", name, wizardp(this_user()) ? "Wizard" : "Mortal");

    outf("%76'-='s-\n", "");

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
    else
    {
	g_info = "";
    }
//### get the eval here...
    outf("Eval: %d   %s\n",
	 1, g_info);
//### xp, advancement info
    out("<< xp, advancement info goes here >>\n");
    outf("Align: %s\n",
	 "Neutral");

    outf("%77'-'s\n", "");

    outf("Hp: %d (%d)   Sp: %d (%d)   %20s\n",
	 this_body()->query_hp(), this_body()->query_max_hp(),
	 1, 1,
	 "Race: " + (capitalize(this_body()->query_race()) || "none"));
    outf("Basic:   Str: %-3d   Agi: %-3d  Int: %-3d  Wil: %-3d\n",
	   this_body()->query_str(), this_body()->query_agi(),
	   this_body()->query_int(), this_body()->query_wil());
    outf("Derived: Con: %-3d   Wis: %-3d  Cha: %-3d\n",
	   this_body()->query_con(), this_body()->query_wis(),
	   this_body()->query_cha());

    outf("%77'-'s\n", "");

//### other misc stats (e.g. sober, poison, wimpy, etc)
    out("<< other misc data: sober, poison, cash, etc >>\n");

    outf("%76'-='s-\n", "");

#endif
}
