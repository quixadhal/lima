/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_exits( ([
  "up" : "tunnel3.scr",
  "down" : "tunnel9.scr",
]) );
set_brief("Shaft");
set_long("In a shaft going straight down.");
}

