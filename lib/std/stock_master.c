/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stock_master.c -- stock guildmaster inherit
**
** 951231, --OH. created, based on DB's Fighter Guildmaster.
*/

inherit MONSTER;
inherit M_GUILD_MASTER;
inherit M_REGEX;

void setup( string guildname )
{
    object sword;


    set_name("Guildmaster");
    set_proper_name("The Guildmaster");
    add_id("master");
    set_id("guildmaster");
    set_gender(1);
    set_in_room_desc( "The " + GUILD_D->query_guild_title( guildname )+ " guildmaster is here." );
    set_long("The guildmaster is a large, muscle-bound killing machine. It would be wise to avoid pissing him off. Ask him to \"join the guild\" or to \"leave the guild\".");
    set_max_hp(10);
    set_which_guild( guildname );

    sword = new("/domains/std/objects/sword");
    sword->move(this_object());
    do_wield(sword);
}

void receive_outside_msg(string str)
{
    if ( search(str, "join +(|the +)guild") != -1 )
    {
	call_out((: add_a_member, this_body() :), 0);
    }
    if ( search(str, "leave +(|the +)guild") != -1 )
    {
        if( !this_body()->query_member_guild( query_which_guild()))
	{
            call_out((: release_a_member, this_body() :), 0);
	}
	else
	{
            call_out((: this_body()->simple_action("The Guildmaster says to $n, \"You cannot leave us, my friend.\""):), 0);
	}
    }
}


void guildmsg_refuse_entry(object who, string why)
{
    targetted_action("The Guildmaster says to $t, \"You already belong to a guild\"", who, why);
}
