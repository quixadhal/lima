/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_WIELDABLE;
inherit M_GETTABLE;
inherit M_MESSAGES;

void mudlib_setup()
{
    ::mudlib_setup();
    add_id("weapon");
    set_gettable(1);
}

void remove() {
    object::remove();
    m_wieldable::remove();
}	    
