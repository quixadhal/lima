/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

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


void setup(){
    object door;

    set_area("wiz_area");
    set_brief("Grand Hall");
    set_long(
"The Grand Hall, the meeting place for Lima Wizards, is a large room with polished wooden floorboards, and rough hewn beams overhead. A narrow flight of stairs lead upwards, their top splashed by flickering blue light, while an equally narrow flight leads downwards into the gloom. Two rocky passages leave the room. The northwest one is warm and sulfurous, while the south passage smells faintly malodorous, as though something had died in the recent past.

A low doorway in the east wall allows access to the example room, a glowing portal in the north wall leads to the mortal start area, and to the west is the quiet room. Its door is currently "

);
    set_state_description( "oak_door_off", "closed.\n");
    set_state_description( "oak_door_on", "open.\n");
    set_exits( ([
		 "east" : "example_room1.c",
		 "south" : "monster_room.c",
		 "west" : "quiet_room.c",
		 "north" : START,
		 "northwest" : "lava_room",
    ]) );
    set_objects( ([
        STAIRS : ({ "/domains/std/attic", "/domains/std/shop", 1}),
           "/domains/std/magic_torch" : 1,
    "/domains/std/large_oak_door" : ({ "west" }),
    ]) );
   set_default_exit( "Walking through walls is painful. Try a more pleasant direction.\n");
door = present( "door");
    if( !door->query_closed())
    door->do_on_open();
}

int sound ()
{
  write ("This is an example sound.  Only you are getting this msg, so "
	 "I guess you're \nhearing voices.\n");
  return 1; // Let the parser know the listen was successfull
}
