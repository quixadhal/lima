// Beek - a simple room to demonstrate relationships and some toys

#include <mudlib.h>

inherit ROOM;

void
create() {
    ::create();
    
    set_brief("Barney's Cave");
    set_light(1);
    set_long("You are standing in Barney's Cave.  It is dominated by the "
	     "presence of it's distinguished occupant.");
    set_exits( ([ "north" : "room2" ]) );
    set_objects( ([
		   "/domains/std/barney" : 1
		   ]) );
}

