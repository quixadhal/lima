/* Do not remove the headers from this file! see /USAGE for more info. */

#define DEBUG
#include <assert.h>

varargs static void cast_spell (object, mixed);

private static string spell_name = split_path (base_name (this_object()))[1];

static void set_spell_name (string s)
{
  spell_name = s;
}

string get_spell_name ()
{
  return spell_name;
}


// You could check for things in these f()'s such as: does the person have the
// necessary components?  Does the person have the spell in memory, or
// is a scroll present?
// Return a string error message or 1 if it is cool.

static mixed valid_reagent (object reagent)
{
  if (reagent)
    {
      return 0;
    }
  return 1;
}

static mixed valid_target (object target)
{
  if (target)
    {
      return 0;
    }
  return 1;
}

static mixed valid_circumstances (object target, object reagent)
{
  return 1;
}

static mixed can_cast_spell (object target, object reagent)
{
  mixed		res;

  res = valid_circumstances (target, reagent);
  if (!res)
    {
      return "You are unable.\n";
    }
  else
    {
      if (stringp (res))
	{
	  return res;
	}
      ASSERT (res == 1);
    }
  res = valid_reagent (reagent);
  if (!res) 
    {
      return sprintf ("You can't cast that with %s.\n", reagent->the_short());
    }
  else
    { 
      if (stringp (res))
	{
	  return res;
	}
      ASSERT (res == 1);
    }
  res =	valid_target (target);
  if (!res) 
    {
      return sprintf ("You can't cast that on %s.\n", target->the_short());
    }
  else
    { 
      if (stringp (res))
	{
	  return res;
	}
      ASSERT (res == 1);
    }

  return 1;
}

nomask varargs mixed _can_cast_spell (object target, object reagent)
{
  ASSERT (previous_object () == find_object (SPELL_D));
  return can_cast_spell (target, reagent);
}

// The daemon calls this, we check if it came from the daemon, and then call
// the local (static) cast_spell
nomask void _cast_spell (object target, object reagent)
{
  ASSERT (previous_object () == find_object (SPELL_D));
  cast_spell (target, reagent);
}

nomask void create_spell ()
{
  ASSERT (SPELL_D->register_spell ());
}

