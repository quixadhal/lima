/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_wave_obj(object ob)
{
    return 1;
}

mixed can_wave_obj_at_obj(object ob1, object ob2) {
    return 1;
}

// Anything can be waved at/be waved.  Might not do anything, tho :)
mixed direct_wave_obj(object ob) {
    return 1;
}

mixed direct_wave_obj_at_obj(object ob1, object ob2) {
    return 1;
}

mixed indirect_wave_obj_at_obj(object ob1, object ob2) {
    return 1;
}

mixed do_wave_it(object ob, object with)
{
    if (!try_to_acquire(ob))
	return;
    
    if (!ob->wave(with)) {
	write(this_body()->useless("Waving "+ob->the_short()));
	return 0;
    }
    return 1;
}

void do_wave_obj(object ob) {
    do_wave_it(ob, 0);
}

void do_wave_obj_at_obj(object ob1, object ob2) {
    do_wave_it(ob1, ob2);
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ", "OBJ at OBJ" }) });
}
