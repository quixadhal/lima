/* Do not remove the headers from this file! see /USAGE for more info. */

#include <hooks.h>

varargs mixed call_hooks(string, mixed, mixed);
int query_attack_speed();
string query_random_armor_slot();
object query_target();
object get_target();
object query_weapon();
void add_event(object target, object weapon, mixed target_extra,
	       mixed data);
int chance_to_hit(object, object);
int disarm_chance(object);
int calculate_damage(object, object);
int badly_wounded();
int panic();
varargs void stop_fight(object);
int check_condition(int);
void simple_action(string);
int query_ghost();
void target_is_asleep();
void handle_events();
void try_to_ready();
object * query_readied();
void remove_readied(object);

void dispatch_opponent()
{
  add_event(query_target(), query_weapon(), 0, "dispatch");
}

void take_a_swing(object target)
{
  object weapon = query_weapon();
  int chance, roll;

// This handles loadable weapons - eg bows
// If they aren't loaded - load them and return
// If they are, treat the ammunition as the weapon for combat
  if(weapon->query_loadable())
  {
    if(!sizeof(query_readied())) // Any ammunition selected and ready ?
    {
      try_to_ready();            // Try to ready ammunition for next round
      return;
    }
    if(weapon->query_loaded())      // Is weapon ready to fire ?
    {
      weapon->discharge(target);    // Call discharge() in weapon
      weapon = query_readied()[0];  // Use ammo as weapon
      remove_readied(weapon);       // Remove ammunition from readied list
      weapon->discharge(target);    // Call discharge() in ammunition
    } else {
      weapon->load();               // continue loading
      return;
    }
  }

  chance = chance_to_hit(weapon, target);

  if(chance < 2)
    chance = 2;
  if(chance > 98)
    chance = 98;

  if(badly_wounded() && random(3) == 0)
    panic();
#if 0
    switch(panic())
    {
      case 2:
        stop_fight();
/* falls through */
       case 1:
        return 0;
    }
#endif /* 0 */

  roll = random(100);
  if(roll > chance)
  {
    if(random(100) < disarm_chance(target) && target->query_weapon() != target)
      add_event(target, weapon, 0, "disarm");
    else
     add_event(target, weapon, 0, "miss");
  } else {
    add_event(target,
              weapon,
              query_random_armor_slot(),
              calculate_damage(weapon, target));
  }
}

void attack()
{
  object target;
  mixed tmp;

  tmp = call_hooks("prevent_combat", (: $1 + "|" + $2 :));
  if(sizeof(tmp))
  {
    tmp = explode(tmp, "|");
    tmp -= ({ "0" });
    tmp = format_list(tmp);
    write("Attack prevented by " + tmp + ".\n");
    return;
  }

/* check whether we are capable of attacking */
  if(tmp = check_condition(1))
  {
    if(tmp[<1] == '\n')
      write(tmp);
    else
      simple_action(tmp + " so $p blows are ineffective.");
    return;
  }

  for(int n = 0; n < query_attack_speed(); n++)
  {
/* any reason to continue the carnage? */
    if(query_ghost() || !(target = get_target()))
    {
      stop_fight();
      return;
    }
    
    if(target->query_asleep())
    {
/* Our target is unconscious. We get to have our way with them
 * *evil grin*
 */
      target_is_asleep();
      return;
    }

    take_a_swing(target);
    handle_events();
  }
}
