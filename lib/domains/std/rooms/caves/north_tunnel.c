/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

void setup()
{
    set_area("pirate");
    set_brief("North Tunnel");
    set_long("This is a tunnel leding north fro the inside of the cave. more to follow. ");

    set_objects( (["/domains/std/objects/gate.c":  ({ "south" }) ]) );

    set_exits( ([
	"south" : "/domains/std/rooms/caves/north_cave.c"
	]) );

    set_state_description("gate_open_off", " To the south is a closed rusty gate");
    set_state_description("gate_open_on", " To the south is an open rusty gate.\n");
}
