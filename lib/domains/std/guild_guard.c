/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit GUILD_GUARD;

void setup( string guild )
{
    set_gender( 2 ); // female - overrides default
    set_in_room_desc( "An armoured figure guards the north exit" );
    add_block( "north" );
}
