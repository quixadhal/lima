/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** diagnose.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

mixed can_diagnose()
{
    return 1;
}

void do_diagnose()
{
    write("You are in perfect health. \n");
}

mixed * query_verb_info()
{
   return ({ ({ "" }) });
}
