/* Do not remove the headers from this file! see /USAGE for more info. */

#include "mudlib.h"

inherit CONTAINER;
inherit "/std/living/grammar";
inherit "/std/living/messages";

/* This is a pure 'living' object, not what is traditionally meant
 * by a living object on LPmuds.  Note that find_living() won't
 * necessarily find it b/c the driver may not even know it's 
 * alive.  (Why the driver keeps track of livings is beyond me ...)
 * [ed note: it doesn't any more; find_living() doesn't exist]
 * This is an object than can hold things and do actions.  If you
 * want it to be able to fight, inherit MONSTER instead.
 */

create() {
    container::create();
    set_max_capacity(100);
    set_def_msgs("living-default");
}

string name;

string query_name() { return name; }

void set_name(string n) {
    if (name) remove_id(lower_case(n));
    name = n;
    add_id(lower_case(n));
}

string in_room_desc() {
  return query_in_room_desc();
}

string inventory_header() {
    return capitalize(query_subjective()) + " is carrying:";
}

int is_living() {
  return 1;
}



/* verb interaction */
mixed direct_cross_obj(object ob) {
    return "I don't think " + ob->the_short() + " would appreciate that.\n";
}

mixed indirect_give_obj_to_liv(object ob, object liv) {
    return capitalize(liv->the_short()) + " politely refuses.\n";
}

mixed direct_get_obj(object ob) {
    return "I can't do everything.  If you want to pick up another player, try using your social skills.\n";
}

mixed direct_whisper_liv_str() { return 1; }
mixed direct_whisper_str_to_liv() { return 1; }
mixed direct_whisper_to_liv_str() { return 1; }
