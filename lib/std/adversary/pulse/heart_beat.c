/* Do not remove the headers from this file! see /USAGE for more info. */

// Heartbeat based interface to combat.  It is intended that this will
// be interchangeable with the non_heart_beat based module.

void switch_to(object);
void attack();
object get_target();

nosave int attacking = 0;

void set_attack_speed(int x) {
}

int query_penalty()
{
    return 0;
}

void do_something()
{
   if(!attacking)
   {
      set_heart_beat(0);
      return;
   }

   attack();
}

void heart_beat()
{
    do_something();
}

/* Call this function to make us start a fight with "who".  It's
 * ok if we're already fighting them.  If they aren't the current
 * attacker, then they will be.
 */
varargs void attacked_by(object who, int take_a_swing)
{
   switch_to(who);

   if(!attacking)
   {
      attacking = 1;
      set_heart_beat(1);
      if(take_a_swing)
         do_something();
   }
}

string continue_fight()
{
   if(!get_target())
      return "You aren't attacking anyone.\n";
   // wait for heart_beat
   return "All in good time.\n";
}

void stop_attacking()
{
   attacking = 0;
}
