/* Do not remove the headers from this file! see /USAGE for more info. */

#include <assert.h>

inherit SPELL;


private static int cast_time;

void do_effects(object target, object reagent);

int set_cast_time(int t)
{
    ASSERT(t>=0);
    cast_time = t;
}

int get_spell_level()
{
    return 1000;  // They should probably redefine this function....
}

// Don't allow spells to redefine this now.
// This should drain spell points, etc..
nomask mixed valid_circumstances(object target, object reagent)
{
    return this_body()->query_member_guild("stock-mage") >= get_spell_level();
}

nomask void cast_spell(object target, object reagent)
{
    write("You begin casting...\n");
    call_out((: do_effects :), cast_time, target, reagent);
}
