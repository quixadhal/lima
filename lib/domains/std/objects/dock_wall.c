/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SECRET_DOOR;
inherit M_READABLE;


mixed direct_open_obj()
{
    if(this_object()->query_closed())
	return "There doesn't seem to be a way to open it from here.\n";
    else
	return "It is already open.\n";
}

mixed direct_close_obj()
{
    if( this_object()->query_closed())
	return "It is already closed.\n";
    else
	return "You can't see any way of doing that.\n";
}


void trigger()
{
    QUEST_D->grant_points(this_body(),"pirate:secretPsg");
    call_out( "open_with", 1 );
    call_out( "close", 8 );
}


void setup()
{
    set_id ("wall","inscription", "writing", "passage");
    set_in_room_desc ("There is an inscription carved into the south wall.");
    set_long ("The inscription is carved into the cave wall.  You notice under the inscription the outline of a passage.\n");
    set_text ("It says:\n\tThis space intentionally left blank.");
    set_close_msg( "The passage slides shut.");
    set_open_msg( "The rock underneath the inscription slides open, revealing a passage.\n");
    setup_door("dock wall", "south", "/domains/std/rooms/caves/Navigation_Room");
}
