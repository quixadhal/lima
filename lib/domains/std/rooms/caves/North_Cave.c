/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;


void setup()
{
    set_area("pirate");
    set_brief( "North Cave" );

    set_long ("This is north side of the cavern.  To the south you see a small east/west running river.  To the west and the east the rock walls of the cavern press in ominously.");

    set_objects( (["/domains/std/objects/gate": ({ "north", "North_Tunnel" })]) );

    add_item("wall", "The cave well is your standard everyday rock.");

    set_exits( ([
"south" : "Inside_Cave",
	]) );

    set_state_description("gate_open_off", " To the north is a very old and rusty gate which appears to be blocking the entrance to a tunnel.");

    set_state_description("gate_open_on", "  To the north is very old and rusty gate which is open, allowing access to the tunnel.");
}
