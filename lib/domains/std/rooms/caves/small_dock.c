/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

private static int passage_open = 0;

void close_passage();
void open_passage();

void setup() 
{ 
  string river = "#You would drown in the river.";

  set_area("pirate");
  set_brief("Small dock");

  set_long("The river cuts through the north wall of this cave, and wanders "
	   "through an opening in the east wall.  A rotting wooden dock "
	   "suggests that at one point this cave was likely a secret port, "
	   "perhaps for pirates.\n");
  set_light(0);
  set_exits( ([
	       "north" : river,
	       "east" : river,
	       "northeast" : river,
	       "northwest" : "inside_cave",
	       ]) );

  set_exit_msg("south", "$N $vscurry through the opening.\n");

  set_hidden_exits("north","east", "northeast");
  set_default_exit ("You can't find an exit in that direction.\n");
  add_item("door", "outline", 
	   ([ 
	     "look" :  "The outline is faint.  You see no immediate way "
             "of getting it open.", 
	     "open" : "There doesn't seem to be a way to open it from here."
	    ]));

  set_objects( ([
		 "/domains/std/objects/river" : 1,
		 "/domains/std/objects/dock_wall" : 1,
		 "/domains/std/objects/dock" : 1, 
		 ]) );
}

string swim ()
{
  write("[in the river]\n");
  return present("river", this_object())->swim();
}

string wade ()
{
  write("[in the river]\n");
  return present("river", this_object())->wade();
}

void close_passage()
{
  object o = load_object("/domains/std/rooms/caves/navigation_room.c");
  o->passage_closed();
  delete_exit("south");
  tell_room(this_object(),"The passage slides shut.\n");
  passage_open = 0;
}

void passage_closed()
{
  delete_exit("south");
  tell_room(this_object(),"The passage slides shut.\n");
  passage_open = 1;
}

void open_passage()
{
  object o;

  if(passage_open)
    {
      return;
    }
  this_body()->simple_action("The rock underneath the inscription slides open, revealing a passage.\n");
  QUEST_D->grant_points(this_body(),"pirate:secretPsg");
  o = load_object("/domains/std/rooms/caves/navigation_room.c");
  add_exit("south" , "/domains/std/rooms/caves/navigation_room.c");
  o->passage_opened();
  call_out((:close_passage:),6);
  passage_open = 1;
}

void passage_opened()
{
  tell_room(this_object(), "The rock underneath the inscription slides open, "
	    "revealing a passage.\n");
  add_exit("south" , "/domains/std/rooms/caves/navigation_room.c");
  passage_open = 1;
}
