/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
#undef EXTRA_RESET
#define EXTRA_RESET no_castle_flag=1;
void setup() {
function f;
set_exits( ([
  "west" : "vill_shore.scr",
  "east" : "vill_shore2.scr",
]) );
set_brief("Road");
set_long("You are on a road going out of the village. To the east the road widens out\n as it leads down to the shore. To the west lies the city.");
set_light(1);
}

