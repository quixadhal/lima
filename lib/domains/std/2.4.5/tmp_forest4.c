/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "forest3.scr",
  "west" : "forest5.scr",
  "east" : "forest6.scr",
  "south" : "forest7.scr",
]) );
set_brief("Deep forest");
set_long("You are in the deep forest.");
set_light(1);
}

