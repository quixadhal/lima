/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wieldable.c
**
** A funny name, but basically a modules that provides basic weapon
** functionality (wielding and the effects of being wielded).
**
** ??-Aug-94 Created.  By Beek.  Originally was /std/weapon.
** 12-Dec-94 Converted to modules approach.  Deathblade.
*/

private int wield_bonus;
private string wield_type = "blow";
private object wielded_by;

private string extra_short() {
    if (wielded_by && wielded_by != this_object())
	return "wielded";
}

int adjust_result(int result)
{
    return result;
}

int query_wield_bonus()
{
    return wield_bonus;
}

string query_wield_type()
{
    return wield_type;
}

int valid_wield()
{
    // return 1 if they can wield this.
    return 1;
}

int is_weapon()
{
    return 1;
}

string query_wield_message()
{
    return "$N $vwield a $o.\n";
}

void mark_wielded_by(object which)
{
//    hook_state("extra_short", "wielded, which && which != this_object);
    wielded_by = which;
}

object query_wielded_by()
{
    return wielded_by;
}

mixed ob_state()
{
    return wielded_by;
}
