/* Do not remove the headers from this file! see /USAGE for more info. */

inherit PARAM_OBJ;
inherit M_GETTABLE;

string this_button;

string my_long()
{
    return "It's a small black button that is currently " + (this_button == environment()->get_current_button() ? "down" : "up") + ".";
}


void do_press( string name )
{
    environment()->pop_up_and_replace( this_button );
}


void setup( string button )
{
    set_id( "button", button );
    set_adj( button, "small", "black" );
    set_long( (: my_long :));
    this_button = button;
    set_size(1);
    set_getmsg( "#It's securely held by the recorder.");
}


mapping lpscript_attributes()
{
    return param_obj::lpscript_attributes() + m_gettable::lpscript_attributes();
}
