/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "vill_green.scr",
  "east" : "vill_road1.scr",
]) );
set_brief("Village track");
set_long("A track going into the village. The track opens up to a road to the east\n and ends with a green lawn to the west.");
set_light(1);
}

