/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "mount_top.scr",
]) );
set_brief("Plateau");
set_long("You are on a large, open plateau on top of the mountain.\n The view is fantastic in all directions and the clouds\n that rush past above feels so close you could almost\n touch them. The air here is fresh and clean.");
set_light(1);
}

