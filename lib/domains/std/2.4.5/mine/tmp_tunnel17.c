/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_brief("Tunnel");
set_exits( ([
  "north" : "tunnel19.scr",
  "south" : "tunnel16.scr",
  "west" : "tunnel18.scr",
]) );
set_long("The tunnel into the mines.");
}

