/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;

#define LABYRINTH "/domains/std/rooms/labyrinth.c"

void setup() {
    string desc = "You stand at the entrance of the labyrinth.  ";
    int has_east = LABYRINTH->entrance_has_east();
    int has_south = LABYRINTH->entrance_has_south();

    set_brief("Labyrinth Entrance");
    if (has_east) {
	if (has_south) {
	    desc += "Passages lead east and south, ";
	    add_exit("south", __DIR__ "0,2.c");
	} else {
	    desc += "Passages lead east, ";
	}
	add_exit("east", __DIR__ "2,0.c");
    } else {
	// We know we have at least one exit
	desc += "Passages lead south, ";
	add_exit("south", __DIR__ "0,2.c");
    }
    set_long(desc + "as well as west out of the labyrinth.\n");
    add_exit("west", "/domains/std/wizroom");
}
