/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat.h>
#include <bodyslots.h>

inherit ARMOR;

void set_slot(string);


class combat_result array 
adjust_result(mixed result) {
    foreach (class combat_result res in result) {
	if (!(res->special & RES_NONPHYSICAL) && random(5)) {
	    res->special = 0;
	    res->damage = 0;
	    res->message = "$P blows glance off $p1 admantine platemail, leaving $t completely unharmed!\n";
	}
    }
    return result;
}

void setup() {
    set_adj("admantine");
    set_id("platemail");
    // platemail acts as if it were plural
    set_proper_name("admantine platemail");
    set_size(LARGE);
#ifdef USE_BODYSLOTS
    set_slot(TORSO);
#endif

    // would call set_armor_class() here, but it isn't really necessary
    // since we are overloading adjust_result()

}
