/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane1.scr",
  "north" : "plane3.scr",
  "east" : "plane4.scr",
  "west" : "plane5.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain, extending in all directions.");
set_light(1);
}

