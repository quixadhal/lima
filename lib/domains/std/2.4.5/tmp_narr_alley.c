/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "north" : "vill_road1.scr",
  "east" : "bank.scr",
  "south" : "post.scr",
  "down" : "well.scr",
]) );
add_item("well", "You look down the well, but see only darkness.\n There are some iron handles on the inside.");
set_brief("Narrow alley");
set_long("A narrow alley. There is a well in the middle.");
set_light(1);
}

