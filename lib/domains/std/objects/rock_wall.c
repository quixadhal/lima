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


void update_sibling()
{
    object sibling = get_sibling();
    // Needed for initial load ...\
    if( !sibling )
	return;

    // Magic ...
    if( is_open())
	sibling->do_on_open();
    else sibling->do_on_close();
    ::update_sibling();
}

void do_on_open()
{
    environment( this_object())->set_room_state( "rock_wall");
}

void do_on_close()
{
    environment( this_object())->clear_room_state( "rock_wall");
}

void setup()
{
    set_id ("wall","inscription", "writing", "passage");
set_long( "The rock wall abruptly terminates the short passage.\n");
    set_flag( ATTACHED );
    add_hook( "open", (: do_on_open :));
    add_hook( "close", (: do_on_close :));
    set_closed(1);
    set_close_msg( "The rock slides back, blocking the passageway.\n");
    set_open_msg( "The rock blocking the passage slides open, allowing you to exit northwards.\n");
    setup_door("dock wall", "north");
}
