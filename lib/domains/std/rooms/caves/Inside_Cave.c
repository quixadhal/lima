/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;

void setup() 
{
    string river = "#You would drown in the river.\n";

    set_area ("pirate");

    set_brief("Large cavern");
    set_long("This is a large cavern on the inside of the mountain, "
	     "large enough to hold a small city.  There is a lazy river running "
	     "right through the center of the cavern, flowing west to east.  "
	     "Huge stalactites and stalagmites are everywhere, in some "
	     "areas they have grown together and formed immense columns "
	     "hundreds of feet high.  "
	     "To the south the tunnels seems to narrow to a very small passage."
	     "  The cave continues to the southeast.\n");
    set_light(0);
    set_exits( ([
	"south" : "Low_Crawl",
	"east"  : river,
	"west"  : river,
	"southeast" : "Small_Dock", 
	"north" : "North_Cave",
	]) );
    set_hidden_exits("east","west");
    set_default_exit ("You can't find an exit in that direction.\n");

    add_item( "stalagmite", "stalactite", "stalagmites", "stalagmite",
	      "columns", "column", 
	      "There is nothing special about the stalagmites and stalactites." );

    add_item("cavern", "cave", "It's big." );

    add_item("river", 
	     ([
		 "swim" : "Swiming in the river accomplishes nothing useful.",
		 "wade" : "Wading in the river accomplishes nothing useful.",
		 ]));


    set_objects( ([]) );
}
