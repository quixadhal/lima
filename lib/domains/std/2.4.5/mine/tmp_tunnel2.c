/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_brief("Tunnel");
set_exits( ([
  "south" : "tunnel.scr",
  "north" : "tunnel3.scr",
]) );
set_long("In the tunnel into the mines.");
}

