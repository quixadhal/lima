/* Do not remove the headers from this file! see /USAGE for more info. */

#include <bodyslots.h>

inherit ARMOR;

void set_slot(string);

void setup()
{
  set_adj("admantine");
  set_id("platemail");
// platemail acts as if it were plural
  set_proper_name("admantine platemail");
#ifdef USE_SIZE
  set_size(LARGE);
#endif
#ifdef USE_MASS
  set_mass(LARGE);
#endif
  set_slot("torso");
  set_armor_class(15);
}
