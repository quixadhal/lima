/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_brief("Attic");
set_long("This is the attic of the wizard hall.  Not much here yet :-)");
set_exits( ([
  "down" : "wizroom.scr",
]) );
set_objects( ([
  "/std/stairs" : ({ 0, "/domains/lpscript/wizroom.scr" }),
  "magic_flame.scr" : 1,
]) );
}

