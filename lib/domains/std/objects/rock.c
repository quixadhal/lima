/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;
inherit M_THROWABLE;

void setup() 
{
  set_adj("dull", "ordinary");
  set_id("rock");
  set_weapon_class(5);
  set_long("It's a rather ordinary rock.");
  set_in_room_desc("There's a dull rock here.\n");
#ifdef USE_SIZE
  set_size(SMALL);
#endif
#ifdef USE_MASS
  set_mass(SMALL);
#endif
}
