/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "ruin.scr",
  "west" : "plane2.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain.");
set_light(1);
}

