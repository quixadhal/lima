/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Village green");
set_long("You are at an open green place south of the village church.\n You can see a road further to the east.\n");
set_exits( ([
  "north" : "church.scr",
  "west" : "hump.scr",
  "east" : "vill_track.scr",
]) );
}

