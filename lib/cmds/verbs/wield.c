/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_wield_obj(object ob)
{
   if(!try_to_acquire(ob))
      return;

   this_body()->do_wield(ob);
}

void do_wield_obj_in_str(object ob, string limb)
{
   if(!try_to_acquire(ob))
      return;

   this_body()->do_wield(ob, limb);
}

void create()
{
   add_rules( ({ "OBJ",
#if WIELD_STYLE == WIELD_LIMBS
                 "OBJ in STR",
#endif
                 }) );
}
 
