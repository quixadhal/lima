/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;


void setup()
{
set_adj( "camera", "polaroid" );
    set_id( "button" );
    set_long( "It's a small black button." );
    set_getmsg( "#That would surely break the camera.");
    set_size(SMALL);
}


int press( string s )
{
    environment( this_object())->take_picture();
    return 1;
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
