/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "eastroad3.scr",
  "south" : "eastroad1.scr",
]) );
set_brief("East road");
set_long("East road runs north-south.");
set_light(1);
}

