/* Do not remove the headers from this file! see /USAGE for more info. */

// aggressive.c
// Inheritable for aggressive monsters
// Onyx@Red Dragon
// NOTE: Someone who understands Lima vanilla combat should edit the
// line I indicated to make this compat.
//
// 12 May, 1998: Iizuka updated to support the new adversary code.

void add_hook(string tag, function hook);
void remove_hook(string tag, function hook);
void start_fight(object who);
private void agro_func(object who);
void i_moved();

private object my_loc;
private int aggression_chance = 100;
private nosave function agro_fn = (: agro_func :);
private nosave function moved_fn = (: i_moved :);

//:FUNCTION handle_attack
// By default, this compares a random number to aggression_chance
// and starts attacking based upon them. This function might be
// overloaded to perform racial checks or something though.
protected void handle_attack(object who)
{
   if(random(100) < aggression_chance)
      start_fight(who);
}

// It checks to see which objects in the room are players and attacks them.
private void agro_func(object who)
{
   object item;

   if(who == this_object())
      foreach(item in all_inventory(environment()))
      {
         if(item->attackable() && item != this_object())
	    handle_attack(item);
      }
   else handle_attack(who);
}

// It moves the "object_arrived" hook (which is associated with a room)
// when the aggressive monster moves
void i_moved()
{
   if(my_loc)
      my_loc->remove_hook("object_arrived", agro_fn );

   my_loc = environment();
   my_loc->add_hook("object_arrived", agro_fn );
}

//:FUNCTION set_aggressive
// This function is used to set the aggression chance of a monster in the
// range 0 (never aggressive) to 100 (completely aggressive).
void set_aggressive(int a)
{
   if(!a)
   {
      if(my_loc)
         my_loc->remove_hook("object_arrived", agro_fn );
      if(aggression_chance)
         remove_hook("move", moved_fn);
   }
   else
   {
      if(my_loc)
         my_loc->add_hook("object_arrived",  agro_fn );
      if(!aggression_chance)
         add_hook("move", moved_fn);
   }
   aggression_chance = a;
}

//:FUNCTION query_aggressive
// This function returns 0 for unaggressive monsters, or the chance of 
// aggression for aggressive monsters.
int query_aggressive()
{
   return aggression_chance;
}

void mudlib_setup()
{
  add_hook("move", moved_fn);
}

mapping lpscript_attributes()
{
  return ([
    "aggressive" : ({ LPSCRIPT_INT, "setup", "set_aggressive" }),
    ]);
}
