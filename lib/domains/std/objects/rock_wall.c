/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SECRET_DOOR;


mixed direct_open_obj()
{
    if(this_object()->query_closed())
	return "There doesn't seem to be a way to open it from here.\n";
    else
	return ::direct_open_obj();
}

mixed direct_close_obj()
{
    if( this_object()->query_closed())
	return ::direct_close_obj();
    else
	return "You can't see any way of doing that.\n";
}


void trigger()
{
    call_out( "open_with", 0 );
    call_out( "close", 8 );
}



void setup()
{
    set_id ("wall", "passage");
set_long( "You are unable to quite see where it slides." );
    set_close_msg( "The rock slides back." );
    set_open_msg( "The rock slides open, allowing you to exit northwards.");
    setup_door("dock wall", "north");
}
