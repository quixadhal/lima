/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

void do_wish_for_str(string str) {
    write("That would be nice, wouldn't it?\n");
}

mixed *query_verb_info() 
{
    return ({ ({ "for STR" }) });
}
