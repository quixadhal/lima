/* Do not remove the headers from this file! see /USAGE for more info. */

//*************************************************************/
//*      an example object for lima bean
//*  
//*      zifnab@lima_bean
//*
//*********************************************************************

#include <mudlib.h>
#include <size.h>

inherit CONTAINER;
inherit M_OPENABLE;
inherit M_GETTABLE;
inherit M_LOCKABLE;

void setup()
{
    set_adj("large");
    set_id("safe");

    set_in_room_desc( "\nThere is a large safe in the center of the room.");

    set_gettable("The safe is way too heavy for you to lift. \n");
    set_locked("skeleton");
    set_closed(1);
    set_long("This is a very large safe");
    set_max_capacity(LARGE);
    set_strength_vs_magic(0);
    set_strength_vs_picking(100);
    set_objects (( [ "/domains/std/example_scroll.c" : 1 ] ));
}


mapping lpscript_attributes()
{
    return m_gettable::lpscript_attributes() +
	m_openable::lpscript_attributes();
}
