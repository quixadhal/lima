/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "down" : "ravine.scr",
  "east" : "mount_top2.scr",
]) );
set_brief("Top of mountain");
set_long("You are on top of a mountain. There is a small plateau to the\n east.");
set_light(1);
}

