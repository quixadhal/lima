/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of the Isle of the Magi");
set_long("You are standing on the shore of the Isle of the Magi\n The shore of the island continues north and southwest from here\n To the west, a hill rises up to the ancient ruins of the Tower\n of Arcanarton, the archmage who used to live on this island\n Halfway up the hill you can see some sort of crumbled monument");
set_exits( ([
  "north" : "sislnd4.scr",
  "west" : "sislnd15.scr",
  "southwest" : "sislnd6.scr",
]) );
}

