/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;

mixed can_go_west() {
    if ("/domains/std/2.4.5/elevator.scr"->query_open_at("lima"))
	return 1;
    else
	return "The door is closed.\n";
}


// If the player is in a vehicle, stop them from using the stairs by 
// returning an error message. Otherwise return 1 to allow them up.
mixed can_go_up()
{
    if( environment( this_body())->is_vehicle()) return "Try walking.\n";
    return 1;
}


// Force the player to use the stairs, return 1 to signify termination.
int do_go_up()
{ 
    this_body()->do_game_command( "climb up stairs");
    return 1;
}


// As with can_go_up().
mixed can_go_down()
{
    if( environment( this_body())->is_vehicle()) return "Try walking.\n";
    return 1;
}


// As with do_go_up().
int do_go_down()
{
    this_body()->do_game_command( "climb down stairs" );
    return 1;
}


// Players can always go north, even when in a vehicle, so return 1.
mixed can_go_north(){ return 1; }


// Force the player to enter the portal, return 1 to stop furthur action.
int do_go_north()
{
    this_body()->do_game_command( "enter portal" );
    return 1;
}


void setup()
{
    set_area("wiz_area");
    set_brief("Grand Hall");
    set_long(
	"The Grand Hall, the meeting place for Lima Wizards, is a large room with polished wooden floorboards, and rough hewn beams overhead. A narrow flight of stairs lead upwards, their top splashed by flickering blue light, while an equally narrow flight leads downwards into the gloom. Two rocky passages leave the room. The northwest one is warm and sulfurous, while the air wafting from the south passage smells as though something had recently died there.  For some bizarre reason, there is an elevator door off to the west.

A low doorway in the east wall allows access to the example room, a glowing portal in the north wall leads to the mortal start area, and to the northeast is the quiet room, the door of which is currently $oak_door.  $lamp"
	);
    set_state_description( "oak_door_off", "closed");
    set_state_description( "oak_door_on", "open");
    set_state_description( "lamp_on", "The lamp beside the elevator is lit.");
    set_exits( ([
	"east" : "Example_Room1",
	"south" : "Monster_Room",
	"west" : "2.4.5/elevator.scr",
	"northeast" : "Quiet_Room",
	"northwest" : "Lava_Room",
//      "north" : START,
//      "up"    : "Attic",
//      "down"  : "Shop",
	]) );
    set_objects( ([
STAIRS : ({ "Attic", "Shop", 1 }),
	"large_oak_door" : ({ "northeast" }),
	"portal" :  ({ START }),
	"2.4.5/obj/elevator_door.scr" : ({ "lima" }),
	"2.4.5/obj/elevator_call_button.scr" : ({ "lima" }),
	"greeter" : 1,
	"map" : 1,
	]) );
    set_default_exit( "Walking through walls is painful. Try a more pleasant direction.\n");
set_exit_msg( "east", ({ "$N $vwomble east.", "$N $vfritter away." }));
}

void do_listen()
{
  write ("This is an example sound.  Only you are getting this msg, so "
	 "I guess you're hearing voices.\n");
}

void arrived() {
    if (query_state("lamp"))
        tell_from_inside(this_object(), "The lamp on the button beside the elevator goes out.\n");
    clear_room_state("lamp");
}
