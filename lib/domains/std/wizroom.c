/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("Grand Hall");
    set_long("This is the wizard Grand Hall in the great world of lima. It is a huge cavern with a domed ceiling. This is the place where wizards gather to discuss issues, decide politics, share laughs, fight holy wars and generally waveleg. To the east is the first example room, to the south is the monster cave and to the west is the {news reading/mail posting/quietly thinking} room.");
    set_light(1);
    set_exits( ([
 "east" : "/domains/std/example_room1.c",
 "south" : "/domains/std/monster_room.c",
 "west" : "/domains/std/quiet_room.c",
 "up" : "/domains/std/attic.c"
    ]) );
    set_default_exit( "There are tales of mighty wizards who walk through walls, but unfortunately\nthese are but tales, and without foundation.  Exits to the east, south, and\nwest may prove more fruitful.\n");
    set_objects( ([
	   STAIRS : ({ 1, "/domains/std/attic", 0 })
    ]) );
}


