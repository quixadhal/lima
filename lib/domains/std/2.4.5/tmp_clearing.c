/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "east" : "forest1.scr",
  "west" : "forest2.scr",
  "north" : "plane1.scr",
]) );
set_brief("Clearing");
set_long("A small clearing. There are trees all around you.\n However, the trees are sparse to the north.");
set_light(1);
}

