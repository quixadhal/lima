/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("Bored Room");
    set_long("This is the wizard lounge in the great world of lima.  You see many confused wizards mulling around.  To the east is an arched doorway though which is an example room and a few example objects from the lima mudlib.");
    set_light(1);
    exits = ([
 "east" : "/domains/std/example_room1.c"
    ]);
}


