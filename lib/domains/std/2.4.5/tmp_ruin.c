/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "plane4.scr",
  "north" : "plane8.scr",
  "east" : "plane9.scr",
  "west" : "plane3.scr",
]) );
set_brief("Ruin");
set_long("A very old looking ruin. There is no roof, and no door.");
set_light(1);
set_objects( ([
  "/domains/std/2.4.5/tmp_ruin_0.scr" : 1,
]) );
}

