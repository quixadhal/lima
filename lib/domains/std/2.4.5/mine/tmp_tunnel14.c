/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_exits( ([
  "east" : "tunnel15.scr",
  "west" : "tunnel9.scr",
]) );
set_brief("Tunnel");
set_long("In the tunnel into the mines.");
}

