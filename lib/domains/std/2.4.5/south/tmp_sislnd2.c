/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of the Isle of the Magi");
set_long("You are standing on the shore of the Isle of the Magi\n The shore of the island continues east and west from here\n To the south, a hill rises up to the ancient ruins of the Tower\n of Arcanarton, the archmage who used to live on this island\n");
set_exits( ([
  "south" : "sislnd14.scr",
  "east" : "sislnd3.scr",
  "west" : "sislnd1.scr",
]) );
}

