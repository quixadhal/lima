/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_bite_obj(object ob)
{
    return 1;
}

void do_bite_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;

    ob->eat();
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ" }), ({ "eat" }) });
}
