/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

void do_drink_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;
    ob->drink_it();
}

mixed * query_verb_info()
{
      return ({ ({ "OBJ" }) });
}
