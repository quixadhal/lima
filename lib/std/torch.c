/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;
inherit LIGHT_OBJECT;

int is_flame()
{
   return 1;
}

void mudlib_setup()
{
   light_object::mudlib_setup();
   weapon::mudlib_setup();
   add_save( ({ "num_decays" }) );
   set_decay_auto_remove(1);
   set_id("torch");
   set_fuel(600);
   set_combat_messages("combat-torch");
   set_damage_type("fire");
   set_weapon_class(5);
}
