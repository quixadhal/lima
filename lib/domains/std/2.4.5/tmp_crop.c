/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "vill_shore.scr",
]) );
set_brief("Fields");
set_long("You are in the middle of the fields where the city grows all its crops.\n A road runs north of here.");
set_light(1);
}

