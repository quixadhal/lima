/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** std_priest_spell.c -- standard spell superclass for "stock priests"
**
** This provides a few alternative ideas to those in the "stock-mage" version
** in particular use of a skill-based system rather than guild-level-based
*/

#include <assert.h>
#include <hooks.h>

inherit SPELL;

private nosave int cast_time;
private nosave int diff;
private nosave string skill_used;
private nosave int tag;

// This hook interrupts casting when called -
// eg by moving, casting a new spell, etc
void interrupt_cast();
function interrupt_function = (: interrupt_cast :);

// This hook prevents combat
int no_combat();
function no_combat_function = (: no_combat :);

void do_effects(object target, object reagent);
void do_fail(object target, object reagent);

int no_combat() { return "spell casting"; }

void set_skill_used(string val) { skill_used = val; }
string query_skill_used() { return skill_used; }

void set_difficulty(int val) { diff = val; }
int query_difficulty() { return diff; }

void set_cast_time(int t)
{
  ASSERT(t>=0);
  cast_time = t;
}

void check_success(object target, object reagent)
{
  this_body()->remove_hook("interrupt",interrupt_function);
  this_body()->remove_hook("prevent_combat", no_combat_function);
#ifdef USE_SKILLS
  if(this_body()->test_skill(skill_used, diff))
    do_effects(target, reagent);
  else
    do_fail(target, reagent);
#endif
}

// Don't allow spells to redefine this now.
// This should drain spell points, etc..
nomask mixed valid_circumstances(object target, object reagent)
{
  if (this_body()->query_member_guild("stock-priest") < 1)
    return "You need to be a priest to cast a priest spell!";
  return 1;
}

nomask void cast_spell(object target, object reagent)
{
  this_body()->simple_action("$N $vbegin to cast.");
  tag = call_out((: check_success :), cast_time, target, reagent);
  this_body()->add_hook("interrupt",interrupt_function);
  this_body()->add_hook("prevent_combat",no_combat_function);
}

void interrupt_cast()
{
  this_body()->simple_action("$P spell casting is interrupted\n");
  remove_call_out(tag);
  this_body()->remove_hook("interrupt",interrupt_function);
  this_body()->remove_hook("prevent_combat", no_combat_function);
}

