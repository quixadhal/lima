/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;

void setup() 
{ 
    string river = "#You would drown in the river.\n";

    set_area("pirate");
    set_brief("Small dock");

    set_long("The river cuts through the north wall of this cave, and wanders through an opening in the east wall.  A rotting wooden dock  suggests that at one point this cave was likely a secret port, perhaps for pirates.");
    set_light(0);
    set_exits( ([
	"north" : river,
	"east" : river,
	"northeast" : river,
	"northwest" : "Inside_Cave",
      ]) );

    //  set_exit_msg("south", "$N $vscurry through the opening.\n");

    set_hidden_exits("south", "north","east", "northeast");
    set_default_error ("You can't find an exit in that direction.\n");
    add_item("door", "outline", 
      ([ 
	"look" :  "The outline is faint.  You see no immediate way "
	"of getting it open.", 
	"open" : "There doesn't seem to be a way to open it from here."
      ]));

    set_objects( ([
	"/domains/std/objects/river" : 1,
	"/domains/std/objects/dock_wall" :
	({ "south", "/domains/lima/rooms/caves/Navigation_Room" }),
	"/domains/std/objects/dock" : 1, 
      ]) );
}

string swim ()
{
    write("[in the river]\n");
    return present("river", this_object())->swim( "swim");
}

string wade ()
{
    write("[in the river]\n");
    return present("river", this_object())->swim( "wade" );
}



