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
  this_body()->simple_action("$N $vpress the button next to the passage.\n");
  environment(this_object())->open_passage();
  return 1;
}
