/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "deep_forest1.scr",
  "east" : "plane11.scr",
  "south" : "plane10.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain. There is a forest to the west");
set_light(1);
}

