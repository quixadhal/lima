/* Do not remove the headers from this file! see /USAGE for more info. */

#include <bodyslots.h>

inherit ARMOR;

void set_slot(string);

void setup() {
    set_adj("admantine");
    set_id("platemail");
    // platemail acts as if it were plural
    set_proper_name("admantine platemail");
    set_size(LARGE);
    set_slot("torso");

    // would call set_armor_class() here, but it isn't really necessary
    // since we are overloading adjust_result()

}
