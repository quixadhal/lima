/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;

void setup()
{
    set_id("maps");
    set_long("They have scribblings all over them.");
    set_flag(ATTACHED);
    set_gettable("The maps are securely fastened to the cave wall.");
    set_size(SMALL);
}



