/* Do not remove the headers from this file! see /USAGE for more info. */
//
// By Ohara.

#include <mudlib.h>

inherit ADVERSARY;
inherit M_BLOCKEXITS;

private nosave string guard_for = "<no guild assigned yet>";

private mixed handle_blocks(string dir)
{
    string * allies;

    /* is the player a member of our guild or a member of an ally? */
    if ( this_body()->query_member_guild(guard_for) )
	return 0;

    allies = GUILD_D->query_guild_allies(guard_for);
    if ( allies && sizeof(this_body()->guilds_belong() & allies) != 0 )
	return 0;

    return ::handle_blocks( dir );
}


void mudlib_setup( string guild )
{
    ::mudlib_setup();

    guard_for = guild;

    set_name("guard");
    set_in_room_desc("A guard is here.");
    set_gender(1);     // default male.
    set_block_action("The $n $vpush $t back. \"Guild members only\", $n $vgrowl.");
}
