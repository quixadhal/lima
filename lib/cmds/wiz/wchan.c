/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wchan.c
**
** General wizard channel command.  This provides access to wizard-only
** channels.
**
** 06-Nov-94. Created. Deathblade.
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

    /* all channels thru this ob are wiz_XXX */
    channel_name = "wiz_" + channel_name;

    NCHANNEL_D->cmd_channel(channel_name, arg);
}
