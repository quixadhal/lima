/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;

string this_button;

string my_long()
{
    return "It's a small black button that is currently " + (this_button == environment()->get_current_button() ? "down" : "up") + ".";
}


int press( string name )
{
    environment()->pop_up_and_replace( this_button );
    return 1;
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

object virtual_create(string args)
{
    return new(base_name(), args);
}

mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
