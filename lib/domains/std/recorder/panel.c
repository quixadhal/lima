/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;


void setup()
{
    set_long( "It's a flat metal panel with a row of buttons: play, stop, rewind, fast forward, and record.");
    set_adj( "flat", "metal" );
    set_id( "panel" );
    set_size(1);
    set_gettable( "#It's securely bound to the recorder.");
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
