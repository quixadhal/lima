/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of Crescent Lake");
set_long("You are standing on the shore of Crescent Lake, a beautiful and\n clear lake. Out in the centre of the lake stands the Isle\n of the Magi.\n Trails lead into the forest to the south and east.\n The shore of Crescent Lake continues northeast and southwest\n");
set_exits( ([
  "northeast" : "sshore7.scr",
  "south" : "sforst20.scr",
  "east" : "sforst19.scr",
  "southwest" : "sshore9.scr",
]) );
}

