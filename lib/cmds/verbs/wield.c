/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

inherit VERB_OB;

void do_wield_obj(object ob)
{
   if(!try_to_acquire(ob))
      return;

   this_body()->do_wield(ob);
}

#if WIELD_STYLE == WIELD_LIMBS
void do_wield_obj_in_str(object ob, string limb)
{
   if(!try_to_acquire(ob))
      return;

   this_body()->do_wield(ob, limb);
}
#endif

void create()
{
   add_rules( ({ "OBJ",
#if WIELD_STYLE == WIELD_LIMBS
                 "OBJ in STR",
#endif
                 }) );
}
 
