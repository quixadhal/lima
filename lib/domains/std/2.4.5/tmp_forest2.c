/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_objects( ([
  "/domains/std/2.4.5/tmp_forest2_0.scr" : 1,
]) );
set_exits( ([
  "east" : "clearing.scr",
  "west" : "slope.scr",
]) );
set_brief("In a forest");
set_long("You are in a big forest.");
set_light(1);
}

