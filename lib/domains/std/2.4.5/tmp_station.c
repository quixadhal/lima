/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "up" : "vill_road2.scr",
]) );
set_brief("Central point");
set_long("This is the central point. A lot of traffic seems to have passed through\n here. If you just wait long enough, some transport might pick you up.");
set_light(1);
}

