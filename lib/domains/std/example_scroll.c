/* Do not remove the headers from this file! see /USAGE for more info. */

//*************************************************************/
//*      an example object for lima bean
//*  
//*      zifnab@lima_bean
//*
//*********************************************************************

#include <mudlib.h>
#include <size.h>

inherit OBJ;
 
inherit M_GETTABLE;
inherit M_LOCKABLE;
inherit M_READABLE;

create()
{
    ::create();

    set_id("scroll");
    set_in_room_desc("An old and dusty scroll");
    set_gettable(1);
    set_long("This is an old and very brittle scroll, inlaid with gold letters that you can barely read");
    set_text("Congratulations you have managed to read the scroll and open the safe.  You are rewarded with this scroll.\n");
    set_size(VERY_SMALL);
}

