/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("An old disused well");
set_long("You are halfway up the hill.\n An old, disused well stands here, the roof having fallen in from neglect.\n On top of the hill, to the east, stands the ruins of the tower of\n Arcanarton.\n Paths wind down to the shore of the island to the north and south\n Down on the shore to the west is a small grove\n The well has a ladder runing down into it.");
set_exits( ([
  "north" : "sislnd12.scr",
  "south" : "sislnd8.scr",
  "east" : "sislnd18.scr",
  "west" : "sislnd9.scr",
  "down" : "lair.scr",
]) );
}

