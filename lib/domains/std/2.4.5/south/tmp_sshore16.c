/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of Crescent Lake");
set_long("You are standing on the shore of Crescent Lake, a beautiful and\n clear lake. Out in the centre of the lake stands the Isle\n of the Magi.\n Trails lead into the forest to the south and west.\n The shore of Crescent Lake continues northwest and southeast\n");
set_exits( ([
  "northwest" : "sshore17",
  "south" : "sforst40",
  "southeast" : "sshore15",
  "west" : "sforst34",
]) );
}
