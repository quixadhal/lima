/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CONTAINER;

void
setup() {
    
    set_id("table");
    set_adj("small", "wooden");
    set_long("It's a simple square table, about 2 feet across, with a plywood surface and simple wooden legs.");
//### Extreme nono
    set_in_room_desc("");
    set_preposition("on");
    set_size(LARGE);
    set_objects( ([
		   "/domains/std/objects/shovel" : 1,
		   ]) );
}

	    
