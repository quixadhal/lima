/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of the Isle of the Magi");
set_long("You are standing on the shore of the Isle of the Magi\n A path leads up the hill to the east.\n The shore of the island continues southeast and northwest into a\n small grove from here\n To the north, you can see an old disused well.\n");
set_exits( ([
  "north" : "sislnd17.scr",
  "east" : "sislnd16.scr",
  "souteast" : "sislnd7.scr",
  "northwest" : "sislnd9.scr",
]) );
}

