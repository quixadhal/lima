/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

void setup()
{
    set_brief("Car wash");
    set_long("The car wash is one of the touchless models. ");
    set_exits( ([ "west" : "/domains/std/example_room1.c" ]) );
}
