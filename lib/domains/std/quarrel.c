/* Do not remove the headers from this file! see /USAGE for more info. */

inherit AMMUNITION;

void setup()
{
  set_id("quarrel");
  set_adj("simple");
  set_long("It's a simple quarrel, for use in a crossbow.");
  set_combat_messages("combat-sword");
  set_damage_type("blade");
#ifdef USE_SKILLS
  set_skill_used("combat/archery");
#endif
  set_weapon_class(10);
}
