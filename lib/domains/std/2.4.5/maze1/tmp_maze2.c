/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_brief("A maze");
set_long("In a maze.\n There are four obvious exits: north, south, west and east.");
set_exits( ([
  "north" : "maze1.scr",
  "east" : "maze1.scr",
  "west" : "maze1.scr",
  "south" : "maze1.scr",
]) );
{
switch (random(4)) {
case 0: {
{
add_exit("north", "maze3");
;
}
break; }
case 1: {
{
add_exit("south", "maze3");
;
}
break; }
case 2: {
{
add_exit("east", "maze3");
;
}
break; }
case 3: {
{
add_exit("west", "maze3");
;
}
break; }
}
;
}
;
}

