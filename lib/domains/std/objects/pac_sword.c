/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

inherit SWORD;

#if BLOWS_STYLE == BLOWS_TYPES
#define evt_data(evt)  evt->data[0]
#define decrement_damage(evt)  evt->data[sizeof(evt->data) - 1]--
#else /* BLOWS_STYLE == BLOWS_SIMPLE */
#define evt_data(evt)  evt->data
#define decrement_damage(evt)  evt->data--
#endif

class event_info source_modify_event(class event_info evt)
{
  int opp_health;
  int damage;
  int excess;
  string array system_limbs;

  if(evt->data == "miss" || evt->data == "disarm")
    return evt;


#ifdef HEALTH_USES_LIMBS
  opp_health = evt->target->query_health(evt->target_extra);
#else
  opp_health = evt->target->query_health();
#endif
  damage = evt->data[sizeof(evt->data) - 1];

tell_user("loriel", "Damage before adj : " + damage + "\n");

  if(opp_health <= damage)
  {
    excess = damage - opp_health;
#if HEALTH_STYLE == HEALTH_HITPOINTS
    for(int i = 0; i<=excess;i++)
      decrement_damage(evt);
#else /* HEALTH_STYLE == HEALTH_LIMBS || HEALTH_STYLE == HEALTH_WOUNDS */
    if(evt->target->is_vital_limb(evt->target_extra))
      for(int i = 0; i<excess;i++)
        decrement_damage(evt);
    if(evt->target->is_system_limb(evt->target_extra))
    {
      system_limbs = evt->target->query_system_limbs() - ({ evt->target_extra });
      foreach(string s in system_limbs)
      {
        if(evt->target->query_health(s) != -1)
        return evt;
      }

      for(int i = 0; i<excess;i++)
        decrement_damage(evt);
    }
#endif /* HEALTH_STYLE */
  }

damage = evt->data[sizeof(evt->data) - 1];

tell_user("loriel", "Damage after adj : " + damage + "\n");

   return evt;
}

void setup()
{
  set_adj("glowing");
  set_id("sword");
  add_id("mercy");
  add_adj("sword of");
  set_proper_name("the Sword of Mercy");
  set_size(MEDIUM);
}
