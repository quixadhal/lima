/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** diagnose.c
**
*/

inherit VERB_OB;
inherit M_MESSAGES;

int need_to_see() {
    return 0;
}

int need_to_be_alive() {
    return 0;
}

int need_to_think() {
    return 0;
}

void do_diagnose()
{
    if (check_vision() != 1)
	write("You can't see.\n");

    this_body()->my_action(this_body()->diagnose());

    // Deja vu.  Drive the players crazy :-)
    if (random(100) == 0)
	write("You have a funny feeling you've been here before.\n");
}

mixed * query_verb_info()
{
   return ({ ({ "" }) });
}
