/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

void setup(){

    /* ensure this place is lit, regardless of DEFAULT_LIGHT_LEVEL */
    set_light(1);

    set_brief("Grue's Lair");
    set_long("Normally anyone who stumbles into a Grue's nest isn't lucky\n"
	     "enough to tell the tale, but I guess you're just special.\n");
}
