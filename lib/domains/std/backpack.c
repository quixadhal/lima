/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CONTAINER;
inherit M_OPENABLE;
inherit M_WEARABLE;
inherit M_GETTABLE;

void
create() {
    ::create();
    
    set_id(({ "backpack", "pack" }));
    set_untouched_desc("A backpack sits on the center of the table.");
    set_long("It's a simple leather backpack, with a flap and strap allowing you to close it.");
    set_gettable(1);
    set_objects( ([
		   "/domains/std/camera" : 1
		   ]) );
}

	    
