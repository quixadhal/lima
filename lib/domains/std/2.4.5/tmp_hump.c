/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_exits( ([
  "east" : "vill_green.scr",
  "west" : "wild1.scr",
]) );
set_brief("Humpbacked bridge");
set_long("An old humpbacked bridge");
set_objects( ([
  "/domains/std/2.4.5/tmp_hump_0.scr" : 1,
]) );
}

