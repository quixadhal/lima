/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane2.scr",
  "north" : "plane6.scr",
  "east" : "ruin.scr",
  "west" : "plane7.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain. There are some kind of building to the east.");
set_light(1);
}

