/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_brief("Tunnel");
set_exits( ([
  "south" : "tunnel17",
  "north" : "tunnel22",
]) );
set_brief("Tunnel");
set_long("The tunnel splits up in a fork forward.");
}

