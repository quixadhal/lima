/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_brief("A maze");
set_long("In a maze.\n There are four obvious exits: north, south, west and east.");
set_exits( ([
  "north" : "maze3.scr",
  "east" : "../well.scr",
  "west" : "../well.scr",
  "south" : "maze2.scr",
]) );
{
switch (random(4)) {
case 0: {
{
add_exit("north", "maze5");
;
}
break; }
case 1: {
{
add_exit("south", "maze5");
;
}
break; }
case 2: {
{
add_exit("east", "maze5");
;
}
break; }
case 3: {
{
add_exit("west", "maze5");
;
}
break; }
}
;
}
;
}

