/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Halfway up the hill on the Isle of the Magi");
set_long("You are halfway up the hill.\n On top of the hill, to the south, stands the ruins of the tower of\n Arcanarton.\n The bridge to the mainland stands at the base of the hill to the north\n A path heads around the hill to the east, and down the hill to the shore\n of the island to the west.");
set_exits( ([
  "north" : "sislnd1.scr",
  "south" : "sislnd18.scr",
  "east" : "sislnd14.scr",
  "west" : "sislnd12.scr",
]) );
}

