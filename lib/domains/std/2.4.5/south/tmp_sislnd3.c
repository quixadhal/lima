/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Shore of the Isle of the Magi");
set_long("You are standing on the shore of the Isle of the Magi\n The shore of the island continues south and west from here\n To the south, a hill rises up to the ancient ruins of the Tower\n of Arcanarton, the archmage who used to live on this island\n although no track leads directly there from here\n");
set_exits( ([
  "south" : "sislnd4.scr",
  "west" : "sislnd2.scr",
]) );
}

