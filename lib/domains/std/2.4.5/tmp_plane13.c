/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "plane11.scr",
  "south" : "plane8.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain. There is a mountain to the north,\n but it is to steep to climb.");
set_light(1);
}

