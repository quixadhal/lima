/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "orc_vall.scr",
  "east" : "forest2.scr",
  "south" : "forest3.scr",
]) );
set_brief("A slope");
set_long("The forest gets light here, and slopes down to the west.");
set_light(1);
}

