/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of Crescent Lake");
set_long("You are standing on the shore of Crescent Lake, a beautiful and\n clear lake. Out in the centre of the lake stands the Isle\n of the Magi.\n A trail leads into the forest to the east.\n The shore of Crescent Lake continues north and south");
set_exits( ([
  "north" : "sshore2.scr",
  "south" : "sshore4.scr",
  "east" : "sforst11.scr",
]) );
}
