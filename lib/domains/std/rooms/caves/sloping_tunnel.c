/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;


void create() 
{ 
  room::create(); 
  set_brief("Sloping tunnel");

  set_long("Nothing yet.\n");
  set_light(0);
  set_exits( ([
	       "south" : "inside_cave",
	       ]) );
  set_hidden_exits();
  set_default_exit ("You can't find an exit in that direction.\n");

  set_objects( ([

		 ]) );
}


