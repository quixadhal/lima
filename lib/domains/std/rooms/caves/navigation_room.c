/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

int passage_open = 0;

void close_passage();

void create() 
{ 
  room::create(); 
  set_brief("Navigation room");

  set_long("Maps of the seven seas, marked up with what is probably "
	   "travel routes, line the walls of this cave. The ceiling has a "
	   "large painting of the stars.\n"
	   "A small wooden table stands in the center of the room.");
  set_light(0);
  set_exits( ([
	       ]) );
  set_hidden_exits();
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
		 ]) );
}

void open_passage()
{
  object o;

  if(passage_open)
    {
      return;
    }
  this_body()->simple_action("A bit of rock in the north wall slides away, opening the passage.\n");
  o = load_object("/domains/std/rooms/caves/small_dock.c");
  add_exit("north" , "/domains/std/rooms/caves/small_dock.c");
  o->passage_opened();
  call_out((:close_passage:), 3);
  passage_open = 1;
}

void passage_opened()
{
  tell_room(this_object(), "A bit of rock in the north wall slides away, opening the passage.\n");
  add_exit("north" , "/domains/std/rooms/caves/small_dock.c");
  passage_open = 1;
}


void close_passage()
{
  object o = load_object("/domains/std/rooms/caves/small_dock.c");
  o->passage_closed();
  delete_exit("north");
  tell_room(this_object(), "The rock slides back, once again blocking the passage.\n");
  passage_open = 0;
}

void passage_closed()
{
  delete_exit("north");
  tell_room(this_object(), "The rock slides back, once again blocking the passage.\n");
  passage_open = 0;
}
