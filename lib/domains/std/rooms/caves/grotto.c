/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create()
{
  room::create();
  set_brief("Painted grotto");
  set_long("Tumbling waves from the ocean creep into the entrance of this "
           "ornately painted cave.  Though some of the paintings have been "
	   "damaged, apparently from floods, they remain breathtaking "
	   "nonetheless.  The faint glow of natural light illuminates this "
	   "grotto, from the cave entrance to the south.  However, it is faint"
	   " enough that you can't tell if it is sunlight, or moonlight.  You "
	   "can make out what appears to be a low crawl, back in the darker "
	   "recesses on the north side of the cave.");
  set_exits( ([
	       "south" : "../beach/outside_cave.c",
	       "out" : "../beach/outside_cave.c",
	       "north" : "low_crawl.c"
	       ]) );
  set_hidden_exits("south");
  set_default_exit( "There are tales of mighty wizards who walk through walls, but unfortunately\nthese are but tales without foundation.\n");

  set_objects( ([
		 "/domains/std/objects/dead_adventurer" : 1,
		 "/domains/std/monsters/troll" : 1,
		 ]) );

  add_item( "cave", "paint", "paintings", "grotto",
	   "The paintings depict many forms of gruesome death."
	   "  Apparently, you're supposed to be scared.");

  add_item( "light", "It seems to be coming from outside the cave." );
  add_item( "moonlight", "You can't be sure it's moonlight from in here." );
  add_item( "sunlight", "You can't be sure it's sunlight from in here." );
  add_item( "crawl", "low crawl", "It's to the north." );

  add_item( "waves", "water" , 
	   ([
	     "look" : "It's just ordinary ocean water.",
	     "wade" : "That would be a waste of time.",
	     "swim" : "The water is too shallow here.",
	     ]));
  
}


