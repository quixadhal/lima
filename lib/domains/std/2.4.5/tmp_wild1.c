/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_brief("Wilderness");
set_long("You are in the wilderness outside the village.\n There is a big forest to the west.");
set_light(1);
set_exits( ([
  "east" : "hump.scr",
  "west" : "forest1.scr",
]) );
}

