/* Do not remove the headers from this file! see /USAGE for more info. */

// limbs.c - Adversary module to control limb-based health. Body styles
//           are stored in /daemons/body_d.c.

#include <limbs.h>

inherit CLASS_LIMB;
inherit __DIR__ "diagnose_msg";

void die();
varargs void simple_action(string, string);
void update_health();
void save_me();
int do_unwield(string);
int query_asleep();
int query_stunned();

private mapping health = BODY_D->get_body("humanoid");
private nosave int health_time = time();
private int heal_rate = 15;
private int dead = 0;

//:FUNCTION update_body_style
// int update_body_style(string body_style);
// Queries BODY_D for the number and type of limbs that will be used.
// e.g. update_body_style("humanoid") will give the body a torso, head,
// two arms, and two legs.
// Returns 0 if the body style doesn't exist or if it doesn't contain
// at least one vital or system limb.
int update_body_style(string body_style)
{
   mapping new_body = BODY_D->get_body(body_style);

   if(!new_body)
      return 0;

   // Make sure we have at least one vital or system limb..
   if(!filter(keys(new_body), (: ((class limb)$(new_body)[$1])->flags &
         (LIMB_VITAL | LIMB_SYSTEM) :)))
      return 0;

   health = new_body;
   return 1;
}

int is_vital_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_VITAL;
}

int is_system_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_SYSTEM;
}

int is_wielding_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_WIELDING;
}

int is_mobile_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_MOBILE;
}

//:FUNCTION query_limbs
// string array query_limbs();
// Returns a string array containing all limbs that health is applied to.
string array query_limbs()
{
   return keys(health);
}

//:FUNCTION query_wielding_limbs
// string array query_wielding_limbs();
// Returns a string array containing all the limbs that can wield weapons.
string array query_wielding_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags & LIMB_WIELDING :));
}

//:FUNCTION query_vital_limbs
// string array query_vital_limbs();
// Returns a string array containing all the limbs that are considered
// vital for survival. If any one of these limbs is disabled, the
// adversary dies.
string array query_vital_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags & LIMB_VITAL :));
}

//:FUNCTION query_mobile_limbs
// string array query_mobile_limbs();
// Lima doesn't do anything with mobile limbs, but they're provided for
// those who want health of mobile limbs to affect movement and such.
string array query_mobile_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags & LIMB_MOBILE :));
}

//:FUNCTION query_system_limbs
// string array query_system_limbs();
// Returns a string array of 'system' limbs. When ALL system limbs are
// disabled, the adversary dies.
string array query_system_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags & LIMB_SYSTEM :));
}

//:FUNCTION query_non_limbs
// string array query_non_limbs();
// Returns a list of body parts that are not worth tracking health for.
// Such body parts are defined by having a max_health of -1.
string array query_non_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->max_health == -1 :));
}

void set_heal_rate(int x)
{
   if(x < 0)
      x = 0; /* a number of things depend on this */
   heal_rate = x;
}

int query_heal_rate()
{
   return heal_rate;
}

//:FUNCTION set_max_limb_health
// void set_max_limb_health(string limb, int x);
// Sets the maximum health for a given limb.
void set_max_limb_health(string limb, int x)
{
   class limb tmp = health[limb];
    
   if(!tmp)
      error("Bad limb.\n");
   if(tmp->max_health == -1 || x < 0)
      return;

   update_health();

   tmp->max_health = x;
   tmp->health = x;
}

//:FUNCTION set_max_health
// void set_max_health(int x);
// Set the maximum number of hit points of a monster, and also set it's 
// hit points to the new max
void set_max_health(int x)
{
   int max = 0;

   foreach(string l, class limb d in health)
      if(d->max_health > max)
         max = d->max_health;

   foreach(string l, class limb d in health)
   {
      d->max_health = d->max_health * x / max;
      d->health = d->max_health;
   }
}

//:FUNCTION kill_us
// void kill_us();
// Kills us. =)
void kill_us()
{
   dead = 1;
   die();
}

string query_random_limb()
{
   string array limbs = filter(keys(health),
                        (: ((class limb)health[$1])->health > 0 :));
   return sizeof(limbs) ? choice(limbs) : 0;
}

//:FUNCTION disable_limb
// void disable_limb(string limb);
// Disables a limb. For effects on vital and system limbs, see
// query_vital_limbs() and query_system_limbs().
void disable_limb(string limb)
{
   int dont_kill_me_now = 0;

   if(health[limb]->health == -1)
      return;

   ((class limb)health[limb])->health = 0;
   if(((class limb)health[limb])->flags & LIMB_VITAL)
   {
      kill_us();
   }
   else if(((class limb)health[limb])->flags & LIMB_SYSTEM)
   {
      foreach (string l in query_system_limbs())
      {
         if(((class limb)health[l])->health != 0)
            dont_kill_me_now = 1;
      }
      if(dont_kill_me_now == 1)
         simple_action("$N cannot use $p $o anymore.", limb);
      else
         kill_us();
   }
   else if(((class limb)health[limb])->flags & LIMB_WIELDING)
   {
      simple_action("$N cannot use $p $o anymore.", limb);
      do_unwield(limb);
   }
   else
      simple_action("$N cannot use $p $o anymore.", limb);
}

//:FUNCTION enable_limb
// void enable_limb(string limb);
// Re-enables a disabled limb.
void enable_limb(string limb)
{
   class limb tmp = health[limb];

   if(tmp->health == 0)
      tmp->health = 1;
   health[limb] = tmp;
}

varargs void set_health(string limb, int x)
{
   class limb tmp = health[limb];

   if(!tmp)
      error("Bad limb.\n");
   update_health();

   if(x > tmp->max_health)
      error("Attempt to set health > max_health.\n");
   if(tmp->health < 1 || dead)
      return;

   tmp->health = x;
   if(tmp->health <= 0)
      disable_limb(limb);
}

//:FUNCTION hurt_us
// varargs int hurt_us(int x, string limb);
// Hurt us a specified amount.
varargs int hurt_us(int x, string limb)
{
   class limb tmp;

   if(!limb)
      limb = query_random_limb();
   tmp = health[limb];
   if(!tmp)
      error("Bad limb.\n");
   update_health();

   if(tmp->health < 1 || dead)
      return 0;

   tmp->health -= x;
   if(tmp->health <= 0)
   {
      x += tmp->health; /* negative */
      tmp->health = 0;
      disable_limb(limb);
   }

   return x;
}

//:FUNCTION heal_limb
// protected void heal_limb(string limb, int x);
// Heal us a specified amount, truncating at max_health.
protected void heal_limb(string limb, int x)
{
   class limb tmp = health[limb];
    
   if(!tmp)
      error("Bad limb.\n");

   if(tmp->health == -1)
      return;

   if(tmp->health == 0 || dead)
      x = fuzzy_divide(x, 10);

   tmp->health += x;
   if(tmp->health > tmp->max_health)
      tmp->health = tmp->max_health;
}

//:FUNCTION is_limb
// int is_limb(string s);
// Returns 1 if 's' is a valid limb.
int is_limb(string s)
{
   return !undefinedp(health[s]);
}

//:FUNCTION query_max_health
// varargs int query_max_health(string limb);
// Tells us the maximum health of a given limb.
varargs int query_max_health(string limb)
{
   int x = 1;

   if(limb)
      return is_limb(limb) ? ((class limb)health[limb])->max_health : 0;

   foreach(string l in keys(health))
      if(health[l]->max_health > x)
         x = health[l]->max_health;
   return x;
}

//:FUNCTION heal_us
// varargs void heal_us(int x, string limb);
// Heals all limbs by 'x' amount.
varargs void heal_us(int x, string limb)
{
   if(!limb || undefinedp(limb))
   {
      foreach(limb in keys(health))
         heal_limb(limb, x);
      return;
   }
   else
      heal_limb(limb, x);
}

//:FUNCTION heal_all
// void heal_all();
// Heal us entirely.
void heal_all()
{
   foreach(string l in keys(health))
      if(!health[l]->health)
         enable_limb(l);
   heal_us(query_max_health());
}

//:FUNCTION reincarnate
// void reincarnate();
// Makes us alive again!
void reincarnate()
{
   if(dead)
   {
      foreach(string limb, class limb l in health)
         enable_limb(limb);
      dead = 0;
      health_time = time();
   }
}

void update_health()
{
   if(dead)
      return;

   if(time() != health_time)
   {
      foreach(string limb in keys(health))
         heal_limb(limb, fuzzy_divide((time()-health_time)*heal_rate, 3000));
      health_time = time();
   }
}

//:FUNCTION query_health
// int query_health(string limb);
// Find the current number of hitpoints of a monster
int query_health(string limb)
{
   update_health();
   return ((class limb)health[limb])->health;
}

int query_ghost()
{
   return dead;
}

//:FUNCTION badly_wounded
// int badly_wounded();
// Returns 1 if we're near death.
int badly_wounded()
{
   foreach (string l, class limb lb in health)
   {
      if ((lb->flags & LIMB_VITAL) && (lb->health < lb->max_health/5))
         return 1;
   }
}

string diagnose()
{
   string ret;
   string array damaged_limbs;

   if(query_ghost())
      return "$N $vare dead. Other than that, things are going pretty well for $n.\n";

   if(query_asleep())
      ret = "$N $vare asleep.\n";
   else if(query_stunned())
      ret = "$N $vare stunned.\n";
   else
      ret = "";

   damaged_limbs = filter(query_limbs(),
                   (: query_health($1) < health[$1]->max_health :));
   foreach(string limb in damaged_limbs)
      ret += diagnose_msg(health[limb]->health * 100 / health[limb]->max_health);

   if(ret == "")
      ret = "You are in excellent health.\n";

   return ret;
}

