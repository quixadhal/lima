/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane6.scr",
  "north" : "mount_pass.scr",
  "east" : "plane13.scr",
  "west" : "plane12.scr",
]) );
set_brief("A large open plain");
set_long("A large open plain, There is a mountain to the north.");
set_light(1);
}

