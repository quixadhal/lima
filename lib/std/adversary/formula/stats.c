/* Do not remove the headers from this file! see /USAGE for more info. */

// stats.c - Generic formula module for basing all combat calculations
//           upon bodystats.
// Iizuka: June 3, 1998.

int query_str();
int query_agi();
int query_weapon_class();

int base_chance_to_be_hit()
{
  return 50 - query_agi();
}

int base_chance_to_hit(object target)
{
  return 50 + query_agi();
}

int base_disarm_chance(object target)
{
  return query_str() + query_agi() / 2;
}

int disarm_chance(object target)
{
  return base_disarm_chance(target) - target->base_disarm_chance(this_object());
}

int calculate_damage(object weapon, object target)
{
  if (weapon)
    return random(weapon->query_weapon_class() + query_str()) + 1;
  else
    return random(query_weapon_class() + query_str()) + 1;
}

int chance_to_hit(object weapon, object target)
{
  if(!weapon || weapon == this_object())
    return (target->base_chance_to_be_hit()
           + base_chance_to_hit(target)) / 2;
  else
    return (target->base_chance_to_be_hit() + weapon->chance_to_hit(target)
           + base_chance_to_hit(target)) / 3;
}

