/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("Attic");
    set_long("This is the attic of the wizard hall.  Not much here yet :-)");
    set_light(1);
    set_exits( ([
 "down" : "/domains/std/wizroom.c"
    ]) );
    set_default_exit( "There are tales of mighty wizards who walk through walls, but unfortunately\nthese are but tales, and without foundation.  Exits to the east, south, and\nwest may prove more fruitful.\n");
    set_objects( ([
	   STAIRS : ({ 1, 0, "/domains/std/wizroom" })
    ]) );
}


