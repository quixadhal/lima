#include <mudlib.h>

inherit CONTAINER;

void
create() {
    ::create();
    
    add_id("table");
    set_long("It's a simple square table, about 2 feet across, with a plywood surface and simple wooden legs.");
    set_in_room_desc("Off to the west side of the room, there is a wooden table against the wall.");
    set_preposition("on");
    set_objects( ([
		   "/domains/std/backpack" : 1,
		   "/domains/std/recorder" : 1,
		   ]) );
}

	    
