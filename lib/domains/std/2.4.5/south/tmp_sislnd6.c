/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of the Isle of the Magi");
set_long("You are standing on the shore of the Isle of the Magi\n The shore of the island continues northeast and southwest from here\n To the northwest, a hill rises up to the ancient ruins of the Tower\n of Arcanarton, the archmage who used to live on this island\n To the north, you can see some sort of crumbled monument\n");
set_exits( ([
  "north" : "sislnd5.scr",
  "west" : "sislnd16.scr",
  "northeast" : "sislnd5.scr",
  "southwest" : "sislnd7.scr",
]) );
}

