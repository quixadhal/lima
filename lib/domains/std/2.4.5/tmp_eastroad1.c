/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "eastroad2.scr",
  "south" : "vill_shore.scr",
]) );
set_brief("East road");
set_long("East road runs north-south.");
set_light(1);
}

