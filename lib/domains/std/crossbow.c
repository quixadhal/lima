/* Do not remove the headers from this file! see /USAGE for more info. */

inherit RANGED_WEAPON;

void setup()
{
  set_id("crossbow");
  set_adj("simple");
  set_long("It's a simple crossbow.");

  set_weapon_class(2); // It can be used to bash if it's not loaded
  set_damage_type("blow");
#ifdef USE_SKILLS
  set_skill_used("combat/sword");
#endif
}
