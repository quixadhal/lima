/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;

void setup()
{
    object ob;

    set_adj("polaroid");
    set_id("picture");
    set_gettable(1);
    if ( (ob = this_body()) && (ob = environment(ob)) )
	set_long("Looking at the picture, you see:\n\n" + ob->long());
    else
	set_long("Looking at the picture, you see fuzzy, gray images.  It would seem that it has been erased while you were away from the mud.");
}

