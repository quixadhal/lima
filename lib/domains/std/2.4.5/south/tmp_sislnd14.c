/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Halfway up the hill on the Isle of the Magi");
set_long("You are halfway up the hill.\n On top of the hill, to the southwest, stands the ruins of the tower of\n Arcanarton, but there is no direct route to the top from here.\n To the south stands some sort of crumbling monument.\n A path winds around the hill to the west, and heads down to the shore\n of the island to the north and east");
set_exits( ([
  "north" : "sislnd2.scr",
  "south" : "sislnd14.scr",
  "east" : "sislnd4.scr",
  "west" : "sislnd13.scr",
]) );
}

