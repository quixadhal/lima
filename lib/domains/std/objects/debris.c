/* Do not remove the headers from this file! see /USAGE for more info. */
#include <move.h>

inherit OBJ;
inherit M_GETTABLE;

void setup() {
    set_id("rock", "rocks", "debris", "rubble");
    set_attached(1);
    set_long("The rocks aren't very interesting.");
    set_size(TOO_LARGE);
}

mixed get()
{
    object env = environment();	
    mixed  r;

    if((r = ::get()) == MOVE_OK)
    {
	this_body()->simple_action ("$N $vpick up a rock.");
	new(__DIR__+  "rock")->move(this_body());
	return MOVE_NO_ERROR;
    }
    return r;
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
