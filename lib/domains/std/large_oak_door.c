/* Do not remove the headers from this file! see /USAGE for more info. */

inherit HIDDEN_DOOR;


void do_on_open()
{
    ::do_on_open();
    environment( this_object())->set_room_state( "oak_door");
}

void do_on_close()
{
    ::do_on_close();
    environment( this_object())->clear_room_state( "oak_door");
}

void on_clone()
{
    ::on_clone();
    if( is_open() ) do_on_open();
    else do_on_close();
}

void setup(string dir, string room)
{
    set_id("door");
    set_adj("large oak", "large", "oak");
    set_long("It is about 10 feet tall, and very beautiful.");
    setup_door("large oak door", dir, room);
}
