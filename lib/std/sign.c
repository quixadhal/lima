/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit OBJ;
inherit M_READABLE;
inherit M_GETTABLE;

void mudlib_setup()
{
    set_id("sign");
    set_gettable(0);
    set_getmsg( "#It's best left alone.\n" );

}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_readable::lpscript_attributes() + m_gettable::lpscript_attributes();
}
