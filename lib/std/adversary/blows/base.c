/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

inherit __DIR__ "class_" STRINGIZE(BLOW_MODULE);

varargs int hurt_us(int, string);
varargs void attacked_by(object, int);
string query_random_limb();
void handle_message(string, object, object);
string damage_message(int);
object array query_targets();
object query_weapon();
varargs void unwield(string);
object array event_get_armors(class event_info);

int do_damage_event(class event_info evt)
{
   int x;
   if(evt->data != "miss")
      x = hurt_us(event_damage(evt));
   if(member_array(previous_object(), query_targets()) == -1)
      attacked_by(previous_object(), 0);
   return x;
}

// Modify us (as attacker). Include things such as racial modifiers,
// resistances, etc.
class event_info modify_our_event(class event_info evt)
{
   return evt;
}

class event_info health_modify_event(class event_info evt)
{
   return evt;
}

class event_info armors_modify_event(class event_info evt)
{
   object array armors = event_get_armors(evt);

   if(armors)
      foreach(object ob in armors)
         evt = ob->sink_modify_event(evt);
   return evt;
}

// This is the method that gets called in the target object. Before
// the event does any damage to us, make modifications.
class event_info modify_event(class event_info evt)
{
   evt = health_modify_event(evt);
   evt = armors_modify_event(evt);

   return evt;
}

void handle_result(class event_info evt)
{
   object w;

   if(stringp(evt->data))
   {
      handle_message("!" + evt->data, evt->target, evt->weapon);

      switch (evt->data)
      {
         case "dispatch":
            evt->target->kill_us();
            break;
         case "disarm":
            w = query_weapon();
            unwield();
            w->move(environment());
            break;
         case "miss":
            evt->target->do_damage_event(evt);
            break;
      }
   }
   else
   {
      int percent = event_damage(evt);
      handle_message(damage_message(percent), evt->target, evt->weapon);
      percent = evt->target->do_damage_event(evt);
   }
}

void handle_events()
{
   int i = 0;

   /* walking over the queue is slightly more efficient than deleting the
      first element each time, causing them all to have to move */
   while (i < sizeof(queue))
   {
      queue[i] = modify_our_event(queue[i]);
      if(!queue[i])
         continue;
      if(queue[i]->weapon)
      {
         queue[i] = queue[i]->weapon->source_modify_event(queue[i]);
         if(!queue[i])
            continue;
      }
      queue[i] = queue[i]->target->modify_event(queue[i]);
      if(!queue[i])
         continue;

      handle_result(queue[i]);
      i++;
   }

   queue = ({ });
}
