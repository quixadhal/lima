/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stats.c -- print out player stats
**
** ### for now, these are numeric... eventually, we'll move to a system
** ### where they are "fuzzy" and use words
**
** 950813, Deathblade: Created.
*/

#include <mudlib.h>
#include <config.h>

inherit CMD;

private void main(string arg)
{
#ifndef USE_STATS
    write(mud_name() + " has no player statistics, sorry.\n");
#else

    string name;

    printf("%76'-='s-\n", "");

#ifdef USE_TITLES
    name = this_body()->query_title();
#else
    name = this_body()->query_name();
#endif
    printf("%s  (%s)\n", name, wizardp(this_user()) ? "Wizard" : "Mortal");

    printf("%76'-='s-\n", "");

    printf("Eval: %d   Guild: %s   Race: %s\n",
	   1, "Goober", "Slime");
    /* ### xp, advancement info */
    write("<< xp, advancement info goes here >>\n");

    printf("%77'-'s\n", "");

    printf("Hp: %d (%d)\nSp: %d (%d)\nAlign: %s\n",
	   this_body()->query_hp(), this_body()->query_max_hp(),
	   1, 1, "Neutral");
    printf("Basic:   Str: %-3d   Agi: %-3d  Int: %-3d  Wil: %-3d\n",
	   this_body()->query_str(), this_body()->query_agi(),
	   this_body()->query_int(), this_body()->query_wil());
    printf("Derived: Con: %-3d   Wis: %-3d  Cha: %-3d\n",
	   this_body()->query_con(), this_body()->query_wis(),
	   this_body()->query_cha());

    printf("%77'-'s\n", "");

    /* ### other misc stats (e.g. sober, poison, wimpy, etc) */
    write("<< other misc data: sober, poison, cash, etc >>\n");

    printf("%76'-='s-\n", "");

#endif
}
