/* Do not remove the headers from this file! see /USAGE for more info. */

// 04-98: Iizuka updated to conform to the new adversary code.
// 05-98: Iizuka added support for multiple resistances.

inherit CLASS_EVENT_INFO;

private mapping resistances = ([]);
private int armor_class;

//:FUNCTION set_armor_class
//Set the protection of the particular damage sink.  random(class) points
//of damage will be prevented.
void set_armor_class(int x)
{
   armor_class = x;
}

//:FUNCTION query_armor_class
//Query the current armor class of an object.  Higher is better.
int query_armor_class()
{
   return armor_class;
}

class event_info sink_modify_event(class event_info evt)
{
   if(stringp(evt->data))
      return evt;
#if BLOW_STYLE == BLOW_TYPES
   if(member_array(evt->data[0], keys(resistances)) != -1)
      evt->data[1] -= random(resistances[evt->data[0]] + armor_class);
   else
#endif
      evt->data[1] -= random(armor_class);
   if(evt->data[1] < 0)
      evt->data[1] = 0;
   return evt;
}

//:FUNCTION set_resist
//set_resist(type, amt) specifies that for damage of type 'type', the armor
//class should be increased by 'amt'
void set_resist(string type, int amt)
{
    resistances[type] = amt;
}

//:FUNCTION set_resistances
//void set_resistances(mapping) - Set all the resistances an armor
//possesses.
void set_resistances(mapping x)
{
   resistances = x;
}

//:FUNCTION is_armor
//Returns 1 if the object is a damage_sink.
int is_armor()
{
    return 1;
}

mapping lpscript_attributes()
{
   return
   ([
      "armor_class" : ({ LPSCRIPT_INT, "setup", "set_armor_class" }),
   ]);
}
