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



private static mapping msgs = ([]);

varargs void create(mapping long, string array ids, object dest)
{
    parse_init();
    description::create();
    flags::create();
    set_attached(1);
    msgs = long;
    if(mapp(msgs))
      {
	if(msgs["look"])
	  set_long(msgs["look"][<1] == '\n' ? msgs["look"] : 
		   msgs["look"]+"\n");
	if(msgs["adjs"])
	  {
	    arrayp(msgs["adjs"]) ? add_adj(msgs["adjs"]...) : 
	      add_adj(msgs["adjs"]);
	    map_delete(msgs, "adjs");
	  }
      }
    else
      {
	if(arrayp(ids) && sizeof(ids))
	  set_long("You see nothing special about the "+ids[0]+".");
      }
    if(arrayp(ids))
      set_id(ids...);
      
    parse_refresh();
    if(!objectp(dest))
      move_object(previous_object());
    else
      move_object(dest);
}



int is_visible()
{
  return 1;
}

// Here on purpose.
varargs mixed call_hooks(mixed s ...)
{
}


mixed direct_verb_rule(string verb, string rule, mixed args)
{
  string s = msgs[verb];
  if(s)
    return s[<1] == '\n' ? s : s+"\n";
}
