/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Mine entrance");
set_exits( ([
  "south" : "../mount_pass.scr",
  "north" : "tunnel2.scr",
]) );
set_objects( ([
  "/domains/std/2.4.5/mine/tmp_tunnel_0.scr" : 1,
]) );
}

