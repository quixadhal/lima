/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "eastroad4.scr",
  "south" : "eastroad2.scr",
  "west" : "sunalley1.scr",
]) );
set_brief("East road");
set_long("East road runs north-south. Sun alley is to the west.\n");
set_light(1);
}

