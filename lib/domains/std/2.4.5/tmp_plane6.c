/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane3.scr",
  "north" : "plane11.scr",
  "east" : "plane8.scr",
  "west" : "plane10.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain.");
set_light(1);
}

