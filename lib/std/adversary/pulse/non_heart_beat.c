/* Do not remove the headers from this file! see /USAGE for more info. */

// Combat which is _not_ heart_beat based.  It is intended that this will
// be interchangeable with the heart_beat based module.

void switch_to(object);
void attack();
object get_target();

private nosave int  attack_time = time();
private int         attack_speed = 5;
nosave int          attacking = 0;

void remove()
{
    remove_call_out();
}

void set_attack_speed(int x)
{
   attack_speed = x;
}

int query_penalty()
{
   int x = 6 - (time() - attack_time);
   attack_time = time();

   return (x > 0 ? x : 0);
}

void do_something()
{
   if(attack_speed)
   {
      if(!attacking) return;
      call_out( (: do_something :), attack_speed + random(5) - 2);
   }
   attack();
}

/* Call this function to make us start a fight with "who".  It's
 * ok if we're already fighting them.  If they aren't the current
 * attacker, then they will be.  We also take a swing, for good
 * measure :)
 */
varargs void attacked_by(object who, int take_a_swing)
{
   if(!attack_speed)
   {
      if(take_a_swing)
      {
         switch_to(who);
         do_something();
      }
      return;
   }

   switch_to(who);

   if(!attacking)
   {
      attacking = 1;
      if(take_a_swing)
         do_something();
      else
         call_out((: do_something :), attack_speed + random(5) - 2);
   }
}

string continue_fight()
{
   if(!get_target())
      return "You aren't attacking anyone.\n";
   do_something();
}

void stop_attacking()
{
   attacking = 0;
}
