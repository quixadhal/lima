/* Do not remove the headers from this file! see /USAGE for more info. */

#include <setbit.h>

inherit VERB_OB;

/* open <something destroyable> with <weapon> */
mixed direct_open_obj_with_obj(object ob1, object ob2) {
    if (!ob1) return 1;
    return ob1->test_flag(DESTROYABLE);
}

mixed indirect_open_obj_with_obj(object ob1, object ob2) {
    if (!ob2) return 1;
    if (ob2 == this_body()) return "What an odd way to use yourself.\n";
    return ob2->is_weapon();
}

int open(object ob, object with)
{
    if (with) {
	try_to_acquire(with);
	if (with->is_weapon() && ob->test_flag(DESTROYABLE)) {
	    this_body()->simple_action("$N $vchop a $o up into itty bitty pieces with $p $o1 in a rather violent attempt to open it.", ob, with);
	    ob->remove();
	    return 1;
	}
    }
    return ob->open_with(with);
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

array query_verb_info()
{
    return ({ ({ "OBJ", "up OBJ", "OBJ with OBJ" }) });
}
