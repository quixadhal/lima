/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("Grand Hall");
    set_long("This is the wizard Grand Hall in the great world of lima. It is a huge cavern with a domed ceiling. This is the place where wizards gather to discuss issues, decide politics, share laughs, fight holy wars and generally waveleg. To the east is the first example room, to the south is the monster cave and to the west is the {news reading/mail posting/quietly thinking} room.\n"
"A new one-way passage to the north leads to the player start room.");
    set_exits( ([
		 "east" : "example_room1.c",
		 "south" : "monster_room.c",
		 "west" : "quiet_room.c",
		 "north" : START,
		 "down" : "shop.c",
		 "up" : "attic.c"
    ]) );
    set_default_exit( "There are tales of mighty wizards who walk through walls, but unfortunately\nthese are but tales, and without foundation.  Exits to the east, south, and\nwest may prove more fruitful.\n");
    set_objects( ([
	   STAIRS : ({ "/domains/std/attic" }),
           "/domains/std/magic_torch" : 1,
	   "/domains/std/large_oak_door" : ({ "west" }),
    ]) );
    add_item("test",(["look" : "It's a test, ninny!", "get" : "Testing get"]));
}
