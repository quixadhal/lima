/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

mixed can_wear_obj(object ob)
{
    return 1;
}

void do_wear_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;

    ob->wear();
    this_body()->simple_action("$N $vwear $o.\n", ob);
}

mixed *query_verb_rules() {
    return ({ "OBJ" });
}

	
