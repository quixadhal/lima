/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of Crescent Lake");
set_long("You are standing on the shore of Crescent Lake, a beautiful and\n clear lake. Out in the centre of the lake stands the Isle\n of the Magi.\n Trails lead into the forest to the north and west.\n The shore of Crescent Lake continues northeast and southwest");
set_exits( ([
  "north" : "sforst21.scr",
  "west" : "sforst27.scr",
  "northeast" : "sshore23.scr",
  "southwest" : "sshore21.scr",
]) );
}

