/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Halfway up the hill on the Isle of the Magi");
set_long("You are halfway up the hill.\n On top of the hill, to the north, stands the ruins of the tower of\n Arcanarton.\n Paths wind down to the shore of the island to the south, east and west");
set_exits( ([
  "north" : "sislnd18.scr",
  "south" : "sislnd7.scr",
  "east" : "sislnd6.scr",
  "west" : "sislnd8.scr",
]) );
}

