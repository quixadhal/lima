/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

/*
mixed receive_object( object target, string relation )
{
    if( target->is_living())
	return "It'd be best to wait until you're out of the car wash before exiting";
    return 1;
}
*/


void setup()
{
    set_brief("Car wash");
    set_long("The car wash is one of the touchless models. ");
    set_exits( ([ "west" : "/domains/std/example_room1.c" ]) );
}
