/* Do not remove the headers from this file! see /USAGE for more info. */

/* Note: we calculate healing on a "need to know" basis.  This allows us
 * to pretend you've been healing ever N seconds, without wasting CPU time
 * actually incrementing something every N seconds. 
 * With a large number of wounded players/monsters on a MUD, this can be
 * a huge CPU gain.
 */

inherit __DIR__ "diagnose_msg";

void die();
varargs int query_health(string unused);
int query_asleep();
int query_stunned();

private int health = 1;
private int max_health = 1;
private nosave int health_time = time();
private int heal_rate = 15;
private int dead = 0;

void set_heal_rate(int x)
{
   if(x < 0)
      x = 0; /* a number of things depend upon this */

   heal_rate = x;
}

int query_heal_rate()
{
   return heal_rate;
}

//:FUNCTION set_max_health
// void set_max_health(int x);
// Set the maximum number of hit points of a monster, and also set it's 
// hit points to the new max.
void set_max_health(int x)
{
   health = max_health = x;
   health_time = time(); /* old healing irrelevant */
}

//:FUNCTION kill_us
// void kill_us()
// Call this function to kill an adversary completely.
void kill_us()
{
   dead = 1;
   die();
}

//:FUNCTION set_health
// void set_health(int x);
// Set the hit points of a monster.  Usually, you want to use heal_us() or
// set_max_health instead.
// see also: set_max_health
void set_health(int x)
{
   if(x > max_health)
      error("Attempt to set health > max_health.\n");
   if(health == 0)
      return;
   health = x;
   if(health <= 0)
      kill_us();
   else
      health_time = time(); /* old healing irrelevant */
}

//:FUNCTION hurt_us
// varargs int hurt_us(int x, string unused);
// Hurt us a specified amount.
varargs int hurt_us(int x, string unused)
{
   query_health(); /* must update healing first */

   health -= x;
   if(health <= 0)
      kill_us();
   return x;
}

//:FUNCTION heal_us
// void heal_us(int x);
// Heal us a specified amount, truncating at max_health.
void heal_us(int x)
{
    /* DO NOT cause health to be updated in this routine, since this routine
     * is called from query_health().  The fact that the health isn't updated
     * doesn't matter anyway, since the effects of adding are cumulative,
     * and hitting the top has no side effects.  Only subtractions from
     * health need to make sure health is updated (since not-added-yet healing
     * may make a difference in whether you die or not).
     */
   if(health == 0)
      return;
   health += x;
   if(health > max_health)
      health = max_health;
}

//:FUNCTION reincarnate
// void reincarnate();
// Makes us alive again!
void reincarnate()
{
   dead = 0;
   if(health < 1)
   {
      health = 1;
   }
   health_time = time();
}

//:FUNCTION update_health
// Correct the health if necessary 
void update_health()
{
  if(!dead && time() != health_time)
    {
      /* pretend we've been healing ... */
      heal_us(fuzzy_divide((time()-health_time) * heal_rate, 1000));
      health_time = time();
    }
}

//:FUNCTION query_health
// varargs int query_health(string unused);
// Find the current number of hitpoints of a monster
varargs int query_health(string unused)
{
  update_health();
  return health;
}

//:FUNCTION query_max_health
// int query_max_health();
// Find the maximum number of hitpoints of a monster.
int query_max_health()
{
   return max_health;
}

//:FUNCTION heal_all
// void heal_all();
// Heal us completely.
void heal_all()
{
   set_health(query_max_health());
}

int query_ghost()
{
   return dead;
}

// Returns a simple choice of limb (target struck) for combat messages
// Overload it for specific npcs as appropriate, so you don't hit the
// hands of a bird, legs of a snake, etc
string query_random_limb()
{
  return choice( ({ "head", "torso", "arm", "leg" }) );
}

//:FUNCTION badly_wounded
// int badly_wounded();
// Returns 1 if we're nearing death.
int badly_wounded()
{
   return health < max_health / 5;
}

string diagnose()
{
   string ret;

   if(query_ghost())
      return "$N $vare dead. Other than that, things are going pretty well for $n.\n";

   if(query_asleep())
      ret = "$N $vare asleep.\n";
   else if(query_stunned())
      ret = "$N $vare stunned.\n";
   else
      ret = "";

   ret += diagnose_msg(query_health() * 100 / query_max_health());
   return ret;
}
