/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

mixed can_go_west() {
    if ("/domains/std/2.4.5/elevator.scr"->query_open_at("lima"))
	return 1;
    else
	return "The door is closed.\n";
}

mixed can_go_up()
{
    if( environment( this_body())->is_vehicle()) return "Try walking.\n";
    return 1;
}

int do_go_up(){ 
    this_body()->do_game_command( "climb up stairs");
    return 1;
}

mixed can_go_down()
{
    if( environment( this_body())->is_vehicle()) return "Try walking.\n";
    return 1;
}

int do_go_down()
{
    this_body()->do_game_command( "climb down stairs" );
    return 1;
}


mixed can_go_north()
{
    return 1;
}

int do_go_north()
{
    this_body()->do_game_command( "enter portal" );
    return 1;
}


void setup()
{
    object door;

    set_area("wiz_area");
    set_brief("Grand Hall");
    set_long(
	"The Grand Hall, the meeting place for Lima Wizards, is a large room with polished wooden floorboards, and rough hewn beams overhead. A narrow flight of stairs lead upwards, their top splashed by flickering blue light, while an equally narrow flight leads downwards into the gloom. Two rocky passages leave the room. The northwest one is warm and sulfurous, while the south passage smells faintly malodorous, as though something had died in the recent past.  For some bizarre reason, there is an elevator door off to the west.

A low doorway in the east wall allows access to the example room, a glowing portal in the north wall leads to the mortal start area, and to the northeast is the quiet room.  "
	);
    set_state_description( "oak_door_off", "The northeast door is currently closed.\n");
    set_state_description( "oak_door_on", "The northeast door is currently open.\n");
/*
  set_state_description( "oak_door_destroyed_on", "Some vandal has destroyed the door that used to block the northeast exit." );
  */
    set_state_description( "lamp_on", "The lamp beside the elevator is lit.\n");
    set_exits( ([
	"east" : "example_room1.c",
	"south" : "monster_room.c",
	"west" : "2.4.5/elevator.scr",
	"northeast" : "quiet_room.c",
	"northwest" : "lava_room",
	]) );
    set_objects( ([
        STAIRS : ({ "/domains/std/attic", "/domains/std/shop", 1}),
	"magic_torch" : 1,
	"large_oak_door" : ({ "northeast" }),
	"portal" : 1,
	"2.4.5/obj/elevator_door.scr" : ({ "lima" }),
	"2.4.5/obj/elevator_call_button.scr" : ({ "lima" }),
	"greeter" : 1,
	"map" : 1,
	]) );
    set_default_exit( "Walking through walls is painful. Try a more pleasant direction.\n");
}

int sound ()
{
  write ("This is an example sound.  Only you are getting this msg, so "
	 "I guess you're hearing voices.\n");
  return 1; // Let the verb know the listen was successfull
}

void arrived() {
    if (query_state("lamp"))
        tell_from_inside(this_object(), "The lamp on the button beside the elevator goes out.\n");
    clear_room_state("lamp");
}
