/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "down" : "mount_pass.scr",
  "up" : "mount_top.scr",
]) );
set_brief("Ravine");
set_long("You are in a ravine between mountains. It seems to be possible\n to go up from here.");
set_light(1);
}

