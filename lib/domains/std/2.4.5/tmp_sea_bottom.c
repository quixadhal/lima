/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_objects( ([
  "/domains/std/2.4.5/tmp_sea_bottom_0.scr" : 1,
]) );
set_exits( ([
  "up" : "sea.scr",
]) );
set_brief("Sea bottom");
set_long("You are at the bottom of the sea.");
set_light(1);
}

