/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_brief("End of maze");
set_long("The end of the maze.\n There is one obvious exit to the south.");
set_exits( ([
  "south" : "maze4.scr",
]) );
set_objects( ([
  "/domains/std/2.4.5/maze1/tmp_maze5_0.scr" : 1,
]) );
}

