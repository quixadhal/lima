/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

void do_wear_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;

    ob->do_wear();
}

mixed *query_verb_info() {
    return ({ ({ "OBJ" }), ({ "put on" }) });
}

	
