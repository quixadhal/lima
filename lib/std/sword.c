/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;

void mudlib_setup()
{
  ::mudlib_setup();
  set_id("sword");
  set_combat_messages("combat-sword");
  set_damage_type("blade");
#ifdef USE_SKILLS
  set_skill_used("combat/sword");
#endif
}
