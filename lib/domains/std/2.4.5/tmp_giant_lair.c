/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_objects( ([
  "/domains/std/2.4.5/tmp_giant_lair_0.scr" : 1,
]) );
set_exits( ([
  "east" : "giant_path.scr",
  "west" : "giant_conf.scr",
]) );
set_brief("Lair of the Giant");
set_long("There are mountains all around you.");
set_light(1);
}

