/* Do not remove the headers from this file! see /USAGE for more info. */

// body_d.c - Daemon for providing limbs to ADVERSARY objects.
//   Iizuka@Isle of Avalon: Feb. 9, 1997.

#include <limbs.h>

inherit M_DAEMON_DATA;
inherit CLASS_LIMB;

private mapping body_types =
([
   "humanoid" : ([
      "head" : new(class limb, health : 20, max_health : 20, flags : LIMB_VITAL),
      "torso" : new(class limb, health : 25, max_health : 25, flags : LIMB_VITAL),
      "left arm" : new(class limb, health : 15, max_health : 15, flags : LIMB_WIELDING),
      "right arm" : new(class limb, health : 15, max_health : 15, flags : LIMB_WIELDING),
      "left leg" : new(class limb, health : 15, max_health : 15, flags : LIMB_MOBILE),
      "right leg" : new(class limb, health : 15, max_health : 15, flags : LIMB_MOBILE),

      /* Define non-health slots as -1 max_health. We use these for
         wearing objects that don't act as armors: rings, necklaces, etc. */
      "neck" : new(class limb, health : -1, max_health : -1, flags : 0),
      "left hand" : new(class limb, health : -1, max_health : -1, flags : 0),
      "right hand" : new(class limb, health : -1, max_health : -1, flags : 0),
   ]),
]);

mapping get_body(string type)
{
   if(!undefinedp(body_types[type]))
      return copy(body_types[type]);
   else
      return ([]);
}

void add_body(string body_name, mapping body_limbs)
{
   if(body_name && body_limbs)
      body_types[body_name] = body_limbs;
   save_me();
}

void add_limb_to_body(string bname, string limb, int new_health, int
                      new_max, int new_flags)
{
   body_types[bname][limb] += new(class limb,
                                  health     : new_health,
                                  max_health : new_max,
                                  flags      : new_flags);
   save_me();
}

void remove_limb_from_body(string body_name, string limb_name)
{
   map_delete(body_types[body_name], limb_name);
   save_me();
}

void remove_body(string body_name)
{
   map_delete(body_types, body_name);
   save_me();
}
