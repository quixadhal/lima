/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit SIGN;

void setup()
{
    set_adj("small", "weathered");

    set_in_room_desc("There is a small sign here, trenched deeply in the sand.");

    set_long("The sign is quite weathered, but is still readable, nonetheless.\n"); 
    set_text( @ENDMARKER

Warning:

DON'T USE THIS AREA ON YOUR MUD!!!

This area is going to be used (ever so greatly changed) by a mud the LIMA guys 
are making.  We are providing this area as an example only!  You can use the 
code to the extent that you want to code something similar by example, but
otherwise, get your own ideas =P

As for this area, if you want to play it before you read the code, there are
nine rooms and a treasure to find.  See if you can do it...

-- The LIMA guys.
ENDMARKER );   

}

