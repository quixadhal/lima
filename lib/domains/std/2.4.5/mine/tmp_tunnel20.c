/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_brief("Dead end");
set_exits( ([
  "east" : "tunnel21.scr",
]) );
}

