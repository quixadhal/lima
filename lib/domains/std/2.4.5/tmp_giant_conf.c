/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_exits( ([
  "east" : "giant_lair.scr",
]) );
set_brief("Giants conference of human bashing");
set_long("You are at the yearly conference of human bashing,\n organized by the giants.");
set_objects( ([
  "/domains/std/2.4.5/tmp_giant_conf_0.scr" : 1,
  "/domains/std/2.4.5/tmp_giant_conf_1.scr" : 1,
  "/domains/std/2.4.5/tmp_giant_conf_2.scr" : 1,
]) );
}

