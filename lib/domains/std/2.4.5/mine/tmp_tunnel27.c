/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "tunnel26.scr",
]) );
set_brief("Dead end");
set_long("End of tunnel.");
set_light(0);
}

