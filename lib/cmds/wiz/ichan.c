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

inherit CMD;

private void main(string arg)
{
    string channel_name;

    if ( !arg || arg == "" ) {
	resend(CMD_OB_CHAN, arg);
	return;
    }
    
    if ( sscanf(arg, "%s %s", channel_name, arg) != 2 )
    {
	channel_name = arg;
	arg = "";
    }

    /* all channels thru this ob are imud_XXX */
    channel_name = "imud_" + channel_name;

    NCHANNEL_D->cmd_channel(channel_name, arg, 1);
}
