/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** fighter_master.c -- the Fighter Guild's guildmaster
**
** 951228, Deathblade: created
*/

inherit MONSTER;
inherit M_GUILD_MASTER;
inherit M_REGEX;

void create()
{
    object sword;

    ::create();
    set_name("Guildmaster");
    set_proper_name("The Guildmaster");
    add_id("master");
    set_id("guildmaster");
    set_gender(1);
    set_in_room_desc("The Fighter Guild's guildmaster");
    set_long("The guildmaster is a large, muscle-bound killing machine. It would be wise to avoid pissing him off. Ask him to \"join the fighter guild\" or to \"leave the fighter guild\".");
    set_max_hp(10);
    set_which_guild( "fighter" );

    sword = new("/domains/std/objects/sword");
    sword->move(this_object());
    do_wield(sword);
}

void receive_outside_msg(string str)
{
    if ( search(str, "join +(|the +)(fighter|guild)") != -1 )
    {
	call_out((: add_a_member, this_body() :), 0);
    }
    if ( search(str, "leave +(|the +)(fighter|guild)") != -1 )
    {
	call_out((: release_a_member, this_body() :), 0);
    }
}
