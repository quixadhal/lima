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
*/

inherit M_DAMAGE_SOURCE;

void hook_state(string, mixed, int);

static function move_hook = (: unwield_me :);

int valid_wield()
{
    // return 1 if they can wield this.
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

void mark_wielded_by(object which)
{
    hook_state("extra_short", "wielded", which && which != this_object());
    hook_state("move", move_hook, which && which != this_object());

    m_damage_source::mark_wielded_by(which);
}

mixed ob_state()
{
    return query_wielded_by();
}

void do_remove()
{
   unwield_me();
}
int direct_wield_obj()
{
    object who = owner(this_object());

    if (who && who != this_body())
	return 0;

    return 1;
}

int direct_remove_obj()
{
    object who = owner(this_object());

    if(who && who != this_body())
      return 0;
  return 1;
}
