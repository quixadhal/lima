/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** xyzzy.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

mixed can_xyzzy()
{
    return 1;
}

void do_xyzzy()
{
    write("A hollow voice says: Fool!\n");
}

mixed * query_verb_info()
{
    return ({ ({ 0 }), ({ "plugh", "plover" }) });
}
