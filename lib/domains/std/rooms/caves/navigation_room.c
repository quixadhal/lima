/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit INDOOR_ROOM;



void setup() 
{
  set_area("pirate");
  set_brief("Navigation room");

  set_long("Maps of the seven seas, marked up with what is probably "
	   "travel routes, line the walls of this cave. The ceiling has a "
	   "large painting of the stars.\n"
	   "A small wooden table stands in the center of the room.");
  set_light(0);
  set_exits( ([
            "north" : "small_dock",
	       ]) );
    set_hidden_exits( "north" );
  add_item("ceiling", "stars" ,
	   "It is a map of the stars, used for navigation.");
  add_item("legs","leg", "There's nothing special about the table's legs.");
  add_item("surface", "There's nothing special about the table's surface.");
  add_item("passage", "outline", "There is nothing special about the "
	   "passage.");
  add_item("maps",
	   ([ "look" : "They have scribblings all over them.",
	    "get" : "The maps are securely fastened to the cave wall." ]));

  set_objects( ([
		 "/domains/std/objects/navigation_table" : 1,
		 "/domains/std/objects/navigation_button" : 1,
    "/domains/std/objects/rock_wall" : 1,
		 ]) );
}

