/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
mixed can_go_up();
mixed do_go_up();
mixed can_go_down();
mixed do_go_down();
mixed can_go_north();
mixed do_go_north();
mixed sound();

void setup() {
function f;
set_area("wiz_area");
set_brief("Grand Hall");
set_long("The Grand Hall, the meeting place for Lima Wizards, is a large room with polished wooden floorboards, and rough hewn beams overhead. A narrow flight of stairs lead upwards, their top splashed by flickering blue light, while an equally narrow flight leads downwards into the gloom. Two rocky passages leave the room. The northwest one is warm and sulfurous, while the south passage smells faintly malodorous, as though something had died in the recent past.

A low doorway in the east wall allows access to the example room, a glowing portal in the north wall leads to the mortal start area, and to the west is the quiet room.  Its door is currently ");
set_state_description("oak_door_off", "closed.");
set_state_description("oak_door_on", "open.");
set_exits( ([
  "east" : "example_room1",
  "south" : "monster_room",
  "west" : "quiet_room",
  "northwest" : "lava_room",
]) );
set_default_exit("Walking through walls is painful.  Try a more pleasant direction.");
set_objects( ([
  "/std/stairs" : ({ "/domains/lpscript/attic.scr", "/domains/std/shop", 1 }),
  "magic_torch.scr" : 1,
  "large_oak_door.scr" : ({ "west" }),
  "portal.scr" : 1,
]) );
{
if (environment(this_object())) {
{
mixed _door;
_door=present("door", environment(this_object()));
if (!call_other(_door, "query_closed")) {
{
call_other(_door, "do_on_open");
;
}
}
;
}
}
;
}
;
}


mixed can_go_up() {
{
if (call_other(environment(this_body()), "is_vehicle"))
  return "Try walking.\n";
return 1;;
}
;
}


mixed do_go_up() {
{
this_body()->do_game_command("climb up stairs");
return 1;;
}
;
}


mixed can_go_down() {
{
if (call_other(environment(this_body()), "is_vehicle"))
  return "Try walking.\n";
return 1;;
}
;
}


mixed do_go_down() {
{
this_body()->do_game_command("climb down stairs");
return 1;;
}
;
}


mixed can_go_north() {
{
return 1;;
}
;
}


mixed do_go_north() {
{
this_body()->do_game_command("enter portal");
return 1;;
}
;
}


mixed sound() {
{
write("This is an example sound.  Only you are getting this message, so I guess you're hearing voices");
return 1;;
}
;
}

