/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * 
 *  Transient effects inheritable for diseases, intoxication, etc...
 *  Midhir@Mutants and Machinery
 *  July 15 1997
 *
 */

#include <hooks.h>

inherit OBJ;
inherit M_GETTABLE;

int effect_duration, effect_type;
string effect_name;

int is_transient_effect() { return 1; }

//:HOOK another_effect_exists
//Passed the object of the existing effect, used to perform extra
//functionality if another effect with the same type/name exists.
void another_effect_exists(object ob) {
  ob->add_effect_duration(effect_duration);
  this_object()->remove();
}

//:FUNCTION set_effect_duration
//Set the duration of the effect in number of heart_beats
void set_effect_duration(int d) { effect_duration = d; }
int query_effect_duration() { return effect_duration; }
void add_effect_duration(int d) { effect_duration += d; }


//:FUNCTION set_effect_type
//Set the string type of effect this is, used to make sure only
//one object of each type is in one inventory.
void set_effect_type(int t) { effect_type = t; }
int query_effect_type() { return effect_type; }


//:FUNCTION set_effect_name
//Sets the string name of the effect, this isn't currently used for
//anything, but could be used for more descriptive names than set_effect_type.
void set_effect_name(string n) { effect_name = n; }
string query_effect_name() { return effect_name; }


//:FUNCTION do_effect
//overload this function to create effects.
//The first argument is the object being afflicted.
void do_effect(object affected) {}

void heart_beat() {
  if (!environment()->is_living()) set_heart_beat(0);
  else set_heart_beat(1);
  do_effect(environment());
  if (effect_duration <= 0) this_object()->remove();
  effect_duration--;
}

void mudlib_setup() {
  set_attached(1);
  add_save(({"effect_duration"}));
  set_heart_beat(1);
}

varargs mixed move(object dest, string where) {
  foreach (object ob in all_inventory(dest)) {
          if (!ob->is_transient_effect()) continue;
          if (ob->query_effect_type() == effect_type && 
              ob->query_effect_name() == effect_name) {
             call_hooks("another_effect_exists", HOOK_IGNORE, ob);
             return;
          }
          continue;
  }
  set_heart_beat(1);
  return ::move(dest, where);
}

mapping lpscript_attributes() {
  return object::lpscript_attributes()+m_gettable::lpscript_attributes();
}
