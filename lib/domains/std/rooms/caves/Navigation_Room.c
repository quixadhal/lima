/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;


void setup() 
{
    set_area("pirate");
    set_brief("Navigation room");

set_long( "Maps of the seven seas, marked up with what is probably travel routes, line the walls of this cave. The ceiling has a painting of the stars. In the centre of the room is a small table, while on the north wall is a button." );
    set_light(0);
    set_hidden_exits( "north" );
    add_item("ceiling", "stars" ,
      "It is a map of the stars, used for navigation.");
    add_item("legs","leg", "There's nothing special about the table's legs.");
    add_item("surface", "There's nothing special about the table's surface.");

    set_objects( ([
	"/domains/std/objects/navigation_table" : 1,
	"/domains/std/objects/navigation_button" : 1,
	"/domains/std/objects/rock_wall" : 1,
        "/domains/std/objects/maps" : 1,
      ]) );
}

