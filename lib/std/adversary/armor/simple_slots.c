/* Do not remove the headers from this file! see /USAGE for more info. */

#include <bodyslots.h>

nosave mapping slots = allocate_mapping(({ TORSO, HEAD, ARMS, LEGS, HANDS, FEET }), 0);
nosave mapping non_armors = allocate_mapping(({ "left hand", "right hand", "neck" }), 0);

private int wear_non_armor(object what, string sname)
{
   if(undefinedp(non_armors[sname]) || non_armors[sname])
      return 0;

   non_armors[sname] = what;
   return 1;
}

nomask int wear_item(object what, string sname)
{
   if(wear_non_armor(what, sname) == 1)
      return 1;

   if(undefinedp(slots[sname]) || slots[sname])
      return 0;

   slots[sname] = what;
   return 1;
}

nomask int remove_item(object what, string sname)
{
   if(slots[sname])
   {
      slots[sname] = 0;
      return 1;
   }
   if(non_armors[sname])
   {
      non_armors[sname] = 0;
      return 1;
   }
}

nomask mixed get_random_clothing()
{
   return choice(values(slots));
}

nomask int has_body_slot(string what)
{
   return (!undefinedp(slots[what]) || !undefinedp(non_armors[what]));
}

string array query_armor_slots()
{
   return keys(slots);
}

string query_random_armor_slot()
{
   string tmp;

   if(!tmp)
      return 0;

   tmp = choice(keys(slots));
   if(!tmp)
      return 0;

   return tmp;
}

object array event_get_armors()
{
   object array tmp = ({ get_random_clothing() }) - ({ 0 });
   if(!sizeof(tmp))
      return 0;
   else return tmp;
}
