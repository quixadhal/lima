/* Do not remove the headers from this file! see /USAGE for more info. */

void simple_action(string, object);

private object weapon;

varargs void unwield(string unused)
{
   if(weapon)
      weapon->mark_wielded_by(0);

   weapon = 0;
}

varargs void wield(object ob, string unused)
{
   if(weapon)
      unwield();

   weapon = ob;
   weapon->mark_wielded_by(this_object());
}

object query_weapon()
{
   return weapon ? weapon : this_object();
}

int do_wield(object ob)
{
   if(!(ob->valid_wield()))
      return 0;
   simple_action(ob->query_wield_message(), ob);
   wield(ob);
    
   return 1;
}

int do_unwield()
{
   if(!weapon || weapon == this_object())
      return 0;
   if(!(weapon->valid_unwield()))
      return 0;

   simple_action(weapon->query_unwield_message(), weapon);
   unwield();

   return 1;
}

string get_wield_attributes()
{
   return " (wielded)";
}
