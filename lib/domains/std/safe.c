/* Do not remove the headers from this file! see /USAGE for more info. */

//*************************************************************/
//*      an example object for lima bean
//*  
//*      zifnab@lima_bean
//*
//*********************************************************************

#include <mudlib.h>
inherit M_OPENABLE;
inherit M_GETTABLE;
inherit M_LOCKABLE;
inherit CONTAINER;

create()
{
    container::create();
    set_id("safe");
    set_in_room_desc( "\nThere is a large safe in the center of the room.");

    set_gettable(-1);
    set_getmsg("The safe is way too heavy for you to lift. \n");
    set_locked("skeleton");
    set_closed(1);
    set_long("This is a very large safe");

    set_objects (( [ "/domains/std/example_scroll.c" : 1 ] ));
}
