/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("A small grove on the shore of the Isle of the Magi");
set_long("You are standing in a small grove on the shore of the Isle of the Magi\n All of the trees here are either diseased, dead or heavily mutated\n The shoreline continues southeast from here, as well as heading northwest\n to Focus Point.\n The grove continues to the north.\n To the east, you can see an old disused well, and beyond that, on top\n of the hill, stands the ruined tower of Arcanarton\n");
set_exits( ([
  "north" : "sislnd11.scr",
  "east" : "sislnd17.scr",
  "southeast" : "sislnd8.scr",
  "northwest" : "sislnd10.scr",
]) );
}

