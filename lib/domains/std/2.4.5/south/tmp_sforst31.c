/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("A dimly lit forest");
set_long("You are in part of a dimly lit forest.\n Trails lead north, south and east\n");
set_exits( ([
  "north" : "sshore19.scr",
  "south" : "sforst32.scr",
  "east" : "sshore18.scr",
]) );
}

