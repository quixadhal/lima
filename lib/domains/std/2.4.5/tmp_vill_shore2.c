/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "jetty.scr",
  "east" : "jetty2.scr",
]) );
set_brief("Village shore");
set_long("The village shore. A jetty leads out to the east. To the north some stairs\n leads down to the north beach. A road starts to the west\n");
set_light(1);
}

