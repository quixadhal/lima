/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of Crescent Lake");
set_long("You are standing on the shore of Crescent Lake, a beautiful and\n clear lake. Out in the centre of the lake stands the Isle\n the Magi.\n A trail leads into the forest to the east.\n The shore of Crescent Lake continues north and south\n");
set_exits( ([
  "north" : "sshore1.scr",
  "south" : "sshore3.scr",
  "east" : "sforst10.scr",
]) );
}
