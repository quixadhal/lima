/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "east" : "tunnel10.scr",
  "north" : "tunnel12.scr",
]) );
set_brief("Tunnel");
set_long("In the tunnel into the mines.");
set_light(0);
}

