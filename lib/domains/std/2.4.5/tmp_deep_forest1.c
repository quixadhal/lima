/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "east" : "plane12.scr",
]) );
set_brief("Deep forest");
set_long("In the deep forest. The wood lights up to the east.");
set_light(1);
}

