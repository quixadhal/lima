/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane5.scr",
  "north" : "plane10.scr",
  "east" : "plane3.scr",
  "west" : "big_tree.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain. There is a big tree to the west.");
set_light(1);
}

