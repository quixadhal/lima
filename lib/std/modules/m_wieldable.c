/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wieldable.c
**
** A funny name, but basically a modules that provides basic weapon
** functionality (wielding and the effects of being wielded).
**
** ??-Aug-94 Created.  By Beek.  Originally was /std/weapon.
** 12-Dec-94 Converted to modules approach.  Deathblade.
** 22-Oct-95 Beek separated out the combat stuff into M_DAMAGE_SOURCE, since
**           we don't want living objects to have the verb interaction
** 961216, Deathblade: added related skill for the weapon.
** ??-Apr-98 Iizuka updated to support the new adversary code. Limb
**           wielding is now available, and inventory description code has
**           been updated in respect to this.
*/

#include <flags.h>

void unwield_me();
void hook_state(string, mixed, int);
void assign_flag(int which, int state);
void clear_flag(int);

private nosave object wielded_by;
private nosave function move_hook = (: unwield_me :);
#if WIELD_STYLE != WIELD_SINGLE
private string array wielding_limbs;
#endif

int valid_wield()
{
    // return 1 if they can wield this.
    return 1;
}

int valid_unwield()
{
    // return 1 if they can unwield this.
    return 1;
}

string query_wield_message()
{
    return "$N $vwield a $o.\n";
}

string query_unwield_message()
{
    return "$N $vunwield a $o.\n";
}

mixed query_wielding()
{
#if WIELD_STYLE == WIELD_SINGLE
   return 0;
#else
   return wielding_limbs;
#endif
}

object query_wielded_by()
{
   return wielded_by;
}

string wielded_attributes()
{
   return this_body()->get_wield_attributes();
}

varargs void mark_wielded_by(object which, string array limbs...)
{
   wielded_by = which;
#if WIELD_STYLE != WIELD_SINGLE
   wielding_limbs = limbs;
   if(which)
      wielded_by->mark_wielding_limbs(limbs);
#endif
   assign_flag(F_WIELDED, which && which != this_object());
   hook_state("move", move_hook, which && which != this_object());
}

mixed ob_state()
{
   return wielded_by ? wielded_attributes() : 0;
}

void unwield_me()
{
   if(wielded_by)
   {
      wielded_by->do_unwield(this_object());
      wielded_by = 0;
      clear_flag(F_WIELDED);
   }
}

void do_remove()
{
   unwield_me();
}

void remove()
{
   unwield_me();
}

mixed direct_wield_obj()
{
   object who = owner(this_object());

   if(who && who != this_body())
      return 0;

   if(wielded_by != 0)
      return "You're already wielding that!\n";

   return 1; /* Fall through */
}

mixed direct_wield_obj_in_str(object ob, string limb)
{
   object who = owner(this_object());

   if(who && who != this_body())
      return 0;

   if(member_array(limb, who->query_wielding_limbs()) == -1)
      return 0;

   if(!who->query_health(limb))
      return sprintf("Your %s is in no condition to wield that!\n", limb);

   return 1;
}

mixed direct_remove_obj()
{
    object who = owner(this_object());

    if(!query_wielded_by())
       return "You are not wielding that.\n";

    if(who && who != this_body())
      return 0;

    return 1;
}

void internal_setup()
{
#if WIELD_STYLE != WIELD_SINGLE
  this_object()->add_save(({ "wielding_limbs" }));
#endif
}
