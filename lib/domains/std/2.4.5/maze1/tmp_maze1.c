/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_brief("A maze");
set_long("In a maze.\n There are four obvious exits: north, south, west and east.");
set_exits( ([
  "north" : "maze2.scr",
  "east" : "maze2.scr",
  "west" : "maze2.scr",
  "south" : "maze2.scr",
]) );
{
switch (random(4)) {
case 0: {
{
add_exit("north", "../well.scr");
;
}
break; }
case 1: {
{
add_exit("south", "../well.scr");
;
}
break; }
case 2: {
{
add_exit("east", "../well.scr");
;
}
break; }
case 3: {
{
add_exit("west", "../well.scr");
;
}
break; }
}
;
}
;
}

