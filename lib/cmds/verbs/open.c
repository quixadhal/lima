/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_open_obj(object ob)
{
   return 1;
}

mixed can_open_up_obj(object ob) {
    return 1;
}

mixed can_open_obj_with_obj(object ob1, object ob2) {
    return 1;
}

/* open <something destroyable> with <weapon> */
mixed direct_open_obj_with_obj(object ob1, object ob2) {
    return (ob1->test_flag(DESTROYABLE) && ob2->is_weapon());
}

mixed indirect_open_obj_with_obj(object ob1, object ob2) {
    return (ob1->test_flag(DESTROYABLE) && ob2->is_weapon());
}

int open(object ob, object with)
{
    if (with) {
	try_to_acquire(with);
	if (with->is_weapon() && ob->test_flag(DESTROYABLE)) {
	    this_body()->simple_action("$N $vchop a $o up into itty bitty pieces with $p $o1 in a rather violent attempt to open it.\n", ob, with);
	    ob->remove();
	    return;
	}
    }
    ob->open_with(with);
}

void do_open_obj_with_obj(object ob1, object ob2) {
    open(ob1, ob2);
}

void do_open_up_obj(object ob) {
    open(ob, 0);
}

void do_open_obj(object ob) {
    open(ob, 0);
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ", "up OBJ", "OBJ with OBJ" }) });
}
