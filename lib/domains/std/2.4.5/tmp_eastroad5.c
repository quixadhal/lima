/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "eastroad4.scr",
  "west" : "inn.scr",
]) );
set_brief("East road");
set_long("East road runs south from here.\n To the west lies the Eastroad Inn.");
set_light(1);
}

