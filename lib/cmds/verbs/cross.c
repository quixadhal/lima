/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_cross_obj(object ob)
{
    return 1;
}


void do_cross_obj(object ob)
{
    ob->cross();
}

mixed * query_verb_info(string rule)
{
    return ({ ({ "OBJ" }) });
}
