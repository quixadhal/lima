/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "east" : "sunalley1.scr",
]) );
set_brief("Sun alley");
set_long("Sun alley runs east from here.");
set_light(1);
}

