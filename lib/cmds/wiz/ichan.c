/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** ichan.c
**
** General Intermud-3 channel command.  This provides access to Intermud-3
** channels.
**
** 25-May-95.  Deathblade.  Created.
*/

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

inherit DAEMON;

int main(string arg)
{
    string channel_name;

    if ( !arg || arg == "" )
	return CMD_OB_CHAN->main(arg);

    if ( sscanf(arg, "%s %s", channel_name, arg) != 2 )
    {
	channel_name = arg;
	arg = "";
    }

    /* all channels thru this ob are imud_XXX */
    channel_name = "imud_" + channel_name;

    NCHANNEL_D->cmd_channel(channel_name, arg);

    return 1;
}
