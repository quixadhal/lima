/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane11.scr",
  "north" : "mine/tunnel.scr",
  "up" : "ravine.scr",
]) );
set_brief("Mountain pass");
set_long("You are in a pass going into the mountain with a steep slope\n upwards to the north.\n However, the path is barred.\n There is a tunnel entrance to the north.\n It might be possible to climb up, though");
set_light(1);
}

