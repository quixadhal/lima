/* Do not remove the headers from this file! see /USAGE for more info. */

//*************************************************************/
//*      an example object for lima bean
//*  
//*      zifnab@lima_bean
//*
//*********************************************************************


inherit OBJ;
 
inherit M_GETTABLE;
inherit M_READABLE;

void setup()
{
    set_id("scroll");
    set_in_room_desc("An old and dusty scroll");
    set_long("This is an old and very brittle scroll, inlaid with gold letters that you can barely read");
    set_text("Congratulations you have managed to read the scroll and open the safe.  You are rewarded with this scroll.\n");
    set_size(VERY_SMALL);
}

mapping lpscript_attributes()
{
    return m_gettable::lpscript_attributes() + m_readable::lpscript_attributes()+ object::lpscript_attributes();
}
