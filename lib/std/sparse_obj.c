/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust.  This one is for simple items there mainly for ornamentation.

#include <move.h>

inherit "/std/object/description";
inherit "/std/object/flags";
inherit "/std/object/non_object";
inherit "/std/object/vsupport";

//:FUNCTION stat_me
//write() some debugging info about the state of the object
int stat_me() 
{
    write("Sparse OBJ.\n");
    write("IDs: "+implode((mixed)parse_command_id_list(),", ")+"\n");
    write("Plurals: "+implode((mixed)parse_command_plural_id_list(),", ")+"\n");
    write("Adjectives: "+implode((mixed)parse_command_adjectiv_id_list(),", ")+"\n");
    write("Long: \n"+long());
    return 1;
}



private static mapping descs = ([]);

varargs void create(string long, string array ids, object dest)
{
    parse_init();
    description::create();
    flags::create();
    set_attached(1);
    if(strlen(long))
      set_long(long);
    if(arrayp(ids))
      set_id(ids...);
    if(stringp(long))
      foreach(string id in ids)
	descs[id] = long;
      
    parse_refresh();
    if(!objectp(dest))
      move_object(previous_object());
    else
      move_object(dest);
}



string get_item_desc(string id)
{
  return (descs[id]);
}

void add_simple_fake_item(string long, string array ids)
{
      set_id(ids...);
      foreach(string id in ids)
	descs[id] = long;
}

int is_visible()
{
  return 1;
}

// Here on purpose.
varargs mixed call_hooks(mixed s ...)
{
}

string my_name;

int direct_verb_rule(string verb, string rule, mixed args)
{
  for(int i = 0; i < sizeof(args)/2; i++)
    {
      if (args[i] == this_object())
	{
	  my_name = args[sizeof(args)/2 + i];
	}
      break;
    }
}

string the_short()
{
  return "the " + my_name;
}

/* need a set_light() for remove() */
void set_light(int x)
{
}
