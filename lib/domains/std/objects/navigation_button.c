/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;

void setup() {

    set_id("button");
    set_long("The button probably opens up the passage you came through to get here.\n");
    set_in_room_desc("You see a button on the wall, next to the outline of a passage.");
    set_size(VERY_SMALL);
}

int press(string n)
{
    object wall;
    wall = present( "wall", environment( this_object()));
    if( wall->query_closed())
    {
	this_body()->simple_action("$N $vpress the button next to the passage.");
	wall->open_with();
        call_out( (:present( "wall", environment( this_object()))->close() :), 8);
    }
    else
	this_body()->simple_action("$N $vpress the button, but nothing happens.");
    return 1;
}
