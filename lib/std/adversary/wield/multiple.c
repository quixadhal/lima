/* Do not remove the headers from this file! see /USAGE for more info. */

// multiple.c - Module to allow adversaries to wield multiple objects at a
//              time without requiring a limb-based health module.
// July 22, 1998: Iizuka created.

void simple_action(string, object);

private nosave mapping weapons = ([
                                 "right hand" : 0,
                                 "left hand"  : 0,
]);

//:FUNCTION set_wield_slots
// Set the names of wielding slots in adversaries. Any existing value
// will be overwritten. Adversaries have "left arm" and "right arm"
// set as default values normally. Note that this is only available if
// WIELD_MULTIPLE is defined.
void set_wield_slots(string array list)
{
   if(!list)
      return;
   weapons = allocate_mapping(list, 0);
}

private string find_open_wield_slot()
{
   string array tmp = filter(keys(weapons), (: !weapons[$1] :));

   if(!sizeof(tmp))
      return 0;

   return choice(tmp);
}

varargs void wield(object ob, string where)
{
   if(weapons[where] == ob)
      return;

   if(!where)
      where = find_open_wield_slot();
   if(!where)
   {
      write("You have no way of wielding that!\n");
      return;
   }

   if(weapons[where])
      weapons[where]->mark_wielded_by(0);

   weapons[where] = ob;
   ob->mark_wielded_by(this_object(), where);
}

varargs void unwield(string where)
{
   if(!where)
   {
      foreach(where in keys(weapons))
         unwield(where);
      return;
   }

   if(weapons[where])
      weapons[where]->mark_wielded_by(0);
   weapons[where] = 0;
}

varargs object query_weapon(string where)
{
   if(!where)
   {
      string array slots = keys(weapons);
      if(!sizeof(slots))
         return this_object();   // I am my own lethal weapon..

      where = choice(slots);
   }

   return weapons[where];
}

varargs int do_wield(object ob, string slot)
{
   if(!ob->valid_wield())
      return 0;
   if(ob->query_wielded_by() == this_object())
      return 0;
   simple_action(ob->query_wield_message(), ob);

   wield(ob, slot);

   return 1;
}

int do_unwield(mixed where)
{
   if(objectp(where))
   {
      string array slots = filter(keys(weapons),
                                  (: weapons[$1] == $(where) :));
      if(!sizeof(slots))
         return 0;
      where = slots[0];
   }

   if(!weapons[where] || weapons[where] == this_object() ||
      !weapons[where]->valid_unwield())
         return 0;

   simple_action(weapons[where]->query_unwield_message(), weapons[where]);
   unwield(where);

   return 1;
}

string get_wield_attributes()
{
   string ret = " (wielded with ";
   string array wielding_slots = previous_object()->query_wielding();

   if(sizeof(wielding_slots) == 1)
      return sprintf(" (wielded in %s)", wielding_slots[0]);
   if(sizeof(wielding_slots) == 2)
   {
      if(wielding_slots[0][<3..] == "arm" && wielding_slots[1][<3..] == "arm")
      {
         ret += explode(wielding_slots[0], " ")[0] + " and ";
         ret += explode(wielding_slots[1], " ")[0] + ")";
         return ret;
      }
      else
      {
         ret += wielding_slots[0] + " and " + wielding_slots[1] + ")";
      }
   }
   else
   {
      for(int x = sizeof(wielding_slots) - 1; x > 1; x--)
         ret += wielding_slots[x] + ", ";
      ret += wielding_slots[1] + " and " + wielding_slots[0] + ")";
   }
   return ret;
}

string array query_wielding_limbs()
{
   return keys(weapons);
}
