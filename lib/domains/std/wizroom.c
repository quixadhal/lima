/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_short("Bored Room");
    set_light(1);
    exits = ([
    ]);
}

string long(){
    string output;
    output = 
    "This is the spot on the Etherial plane of Atrii that Implementors most\n"
    "prefer to frequent when they're bored silly.";
    if(present("board",this_object())) 
	output += "  The fact that there\n"
	"is a board here is coincidental.  Honest.\n";
    else
	output += "  And with the absence\n"

	"of the board that is supposed to be here, this room seems about as\n"
	"drab as we make them.\n";
    output += show_objects();
    return output;
}

