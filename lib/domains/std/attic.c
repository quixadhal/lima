/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;


mixed can_go_down()
{
    return 1;
}


int do_go_down()
{
    this_body()->force_game_command( "climb down stairs");
    return 1;
}


void setup() {
    set_brief("Attic");
    set_long("This is the attic of the wizard hall.  Not much here yet :-)");
    set_objects( ([
	STAIRS : ({ 1, 0, "/domains/std/wizroom" }),
	"/domains/std/magic_flame" : 1
      ]) );
}
