/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "tunnel20.scr",
  "east" : "tunnel22.scr",
]) );
set_brief("Tunnel");
set_long("Tunnel into the mines.");
set_light(0);
}

