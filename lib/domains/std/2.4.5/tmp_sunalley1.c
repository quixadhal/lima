/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "sunalley2.scr",
  "east" : "eastroad3.scr",
]) );
set_brief("Sun alley");
set_long("Sun alley runs east-west.");
set_light(1);
}

