/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

mixed do_eat_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;
if(ob->eat_it()) return 1;
  return 0;
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ" }) });
}
