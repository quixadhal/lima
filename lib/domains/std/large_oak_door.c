/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SECRET_DOOR;

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
    environment( this_object())->set_room_state( "oak_door");
}

void do_on_close()
{
    environment( this_object())->clear_room_state( "oak_door");
}

void setup(string dir)
{
    set_id("door");
    set_adj("large oak", "large", "oak");
    set_long("It is about 10 feet tall, and very beautiful.\n");
    add_hook( "open", (: do_on_open :));
    add_hook( "close", (: do_on_close :));
    set_closed(1);
    setup_door("large oak door", dir);
}
