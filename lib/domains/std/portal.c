/* Do not remove the headers from this file! see /USAGE for more info. */

inherit PORTAL;

void setup ()
{
    set_flag( ATTACHED );
    set_long( "It's a glowing portal, set into the northern wall.");
    add_adj( "glowing", "northern", "north" );
    set_id( "portal" );
    set_destination( START );
    set_look_in_desc( "You can vaguely see a sandy beach.");
}
