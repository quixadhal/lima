/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("Big tree");
set_long("A big single tree on the plain.");
set_exits( ([
  "east" : "plane7.scr",
  "west" : "giant_path.scr",
]) );
set_objects( ([
  "obj/rope" : 1,
  "/domains/std/2.4.5/tmp_big_tree_0.scr" : 1,
]) );
}

