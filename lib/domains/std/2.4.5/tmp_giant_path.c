/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "east" : "big_tree.scr",
  "west" : "giant_lair.scr",
]) );
set_brief("A path");
set_long("You are on a path going in east/west direction. There are some\n VERY big footsteps here.");
set_light(1);
}

