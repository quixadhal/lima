/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_objects( ([
  "obj/stethoscope" : 1,
  "/domains/std/2.4.5/tmp_plane9_0.scr" : 1,
]) );
set_exits( ([
  "west" : "ruin.scr",
]) );
set_brief("A large open plain.");
set_long("A large open plain. There is a river to the east, and some kind of building to the west");
set_light(1);
}

