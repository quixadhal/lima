/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Link to the mainland");
set_long("You are standing on the shore of the Isle of the Magi\n The shore of the island continues east and southwest from here\n To the south, a hill rises up to the ancient ruins of the Tower\n of Arcanarton, the archmage who used to live on this island\n A magical bridge now stands on the ruins of the old stone bridge\n to the northwest");
set_exits( ([
  "south" : "sislnd13.scr",
  "east" : "sislnd2.scr",
  "northwest" : "sshore26.scr",
  "southwest" : "sislnd12.scr",
]) );
}

