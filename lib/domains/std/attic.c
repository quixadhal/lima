/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

void setup() {
    set_brief("Attic");
    set_long("This is the attic of the wizard hall.  Not much here yet :-)");
    set_exits( ([
		 "down" : "/domains/std/wizroom.c"
    ]) );
    set_objects( ([
	   STAIRS : ({ 0, "/domains/std/wizroom" }),
	   "/domains/std/magic_flame" : 1
    ]) );
}
