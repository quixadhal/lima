/* Do not remove the headers from this file! see /USAGE for more info. */

#ifdef USE_BODYSLOTS
#include <bodyslots.h>
#endif

inherit ARMOR;

void setup() {
    set_adj("chainmail");
    set_id("shirt", "chainmail");
    set_armor_class(4);
    set_resist("blade", 2); // a little better against blades
#ifdef USE_BODYSLOTS
    set_slot(TORSO);
#endif
}
