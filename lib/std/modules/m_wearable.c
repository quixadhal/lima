/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wearable.c
**
** Mixed in to create a wearable item.  The client should also
** mix in M_GETTABLE.
**
** 07-Sep-94. Created. Rust/Belboz.
** 12-Nov-94. Updated for "modules" approach. Deathblade.
** 970421, Deathblade: switched to flags to pick up attributes
** 25 March, 1998: Iizuka made major changes to support the new adversary
                   code.
*/

#include <flags.h>

void hook_state(string, mixed, int);
string worn_extra_short();
int test_flag(int);
void assign_flag(int, int);

private string wearmsg = "$N $vwear a $o.";
private nosave string slot;
private nosave string array also_covering;

mixed ob_state()
{
   if(!test_flag(F_WORN))
      return 0;

   return slot;
}

//:FUNCTION set_wearmsg
//Set the message used when an object is worn.
void set_wearmsg(string s)
{
   wearmsg = s;
}

//:FUNCTION query_wearmsg
//Return the message given when an object is used.
string query_wearmsg()
{
   return wearmsg;
}

//:FUNCTION set_slot
//Set the bodyslot that the wearable object takes up.
void set_slot(string which)
{
   slot = which;
}

//FUNCTION query_slot
//Return the bodyslot the wearable object takes up.
string query_slot()
{
   return slot;
}

string array also_covers()
{
   return also_covering;
}

void set_also_covers(string array s...)
{
   also_covering = s;
}

string worn_attributes()
{
   string array all = ({ slot });

   if(also_covers())
      all += also_covers();
   if(sizeof(all) > 1)
      return "worn over " + implode(all[0..(sizeof(all) - 2)], ", ")
                + ", and " + all[sizeof(all) - 1];
   else
      return "worn over " + all[0];
}

//:FUNCTION set_worn
//set_worn(1) causes an object to become worn.  set_worn(0) removes it.
void set_worn(int g)
{
   assign_flag(F_WORN, g);
   hook_state("prevent_drop", "You'll have to take it off first.\n", g);
}

void remove()
{
   object env = environment();

   if(!slot || !env)
      return 0;
   set_worn(0);
   env->remove_item(this_object(), slot);
}

void do_wear()
{
   if(!slot)
      return;
   if(!environment()->wear_item(this_object(), slot))
   {
      environment()->simple_action("$N $vdiscover $ns cannot wear the $o.",
                     this_object());
      return;
   }

   set_worn(1);
   environment()->simple_action(query_wearmsg(), this_object());
}

void do_remove()
{
   if(!slot)
      return 0;
   environment()->remove_item(this_object(), slot);
   set_worn(0);
   this_body()->simple_action("$N $vremove $p $o.", this_object());
}

mixed direct_wear_obj()
{
   object who = owner(this_object());

   if(who != this_body())
      return 0;
   if(!this_body()->has_body_slot(slot))
      return "You can't seem to find anywhere to put it on!\n";
   if(test_flag(F_WORN))
      return "But you're already wearing it!\n";
   return 1;
}

mixed direct_remove_obj()
{
   object who = owner(this_object());

   if(who != this_body())
      return 0;
   if(environment() != this_body() || !test_flag(F_WORN))
      return "But you aren't wearing it!\n";
   return 1;
}

mapping lpscript_attributes()
{
   return ([
      "bodyslot" : ({ LPSCRIPT_STRING, "setup", "set_slot" }),
         "wearmsg" : ({ LPSCRIPT_STRING, "setup", "set_wearmsg" }),
   ]);
}
