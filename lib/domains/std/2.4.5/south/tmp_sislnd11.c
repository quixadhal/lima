/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("A grove on the shore of the Isle of the Magi");
set_long("You are standing in a grove on the shore of the Isle of the Magi\n All of the trees in the grove are either diseased, dead or heavily mutated.\n The shore of the island continues to the east,and the grove follows\n the shoreline west to Focus Point.\n The grove also continues to the south.\n");
set_exits( ([
  "south" : "sislnd9.scr",
  "east" : "sislnd12.scr",
  "west" : "sislnd10.scr",
]) );
}

