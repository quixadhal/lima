/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "vill_track.scr",
  "north" : "yard.scr",
  "south" : "narr_alley.scr",
  "east" : "vill_road2.scr",
]) );
set_brief("Village road");
set_long("A long road going east through the village. The road narrows to a\n track to the west. There is an alley to the north and the south.");
set_light(1);
}

