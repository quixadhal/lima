/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit LIGHT_OBJECT;
inherit M_SWITCHABLE;

void turn_on()
{
   set_is_on(1);
   do_light();
}

void turn_off()
{
   set_is_on(0);
   do_extinguish();
}

mixed direct_flip_obj_on(object ob)
{
   return ::direct_light_obj(ob);
}

mixed direct_flip_obj_off(object ob)
{
   return ::direct_extinguish_obj(ob);
}

void mudlib_setup()
{
   object::mudlib_setup();
   light_object::mudlib_setup();
   add_save( ({ "num_decays" }) );
   set_id("lantern");
#ifdef USE_SIZE
   set_size(MEDIUM);
#endif
#ifdef USE_MASS
   set_mass(MEDIUM);
#endif
   set_long("It's an unremarkable lantern.");
   set_fuel(3600);
   set_gettable(1);
}
