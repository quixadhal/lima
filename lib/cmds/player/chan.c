/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** chan.c
**
** General channel command.
**
** 06-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(string arg)
{
    string channel_name;
    int chan_type;

    if ( !arg || arg == "" || (wizardp(this_user()) && arg == "-d") )
    {
	string * channel_list;

	channel_list = this_body()->query_channel_list();
	if ( sizeof(channel_list) == 0 )
	    write("You are not listening to any channels.\n");
	else
	{
	    if ( arg != "-d" )
		channel_list = map(channel_list,
				   (: NCHANNEL_D->user_channel_name($1) :));

	    write("You are listening to: " +
		  implode(channel_list, ", ") + ".\n");
	}
				     
	return;
    }

    if ( sscanf(arg, "%s %s", channel_name, arg) != 2 )
    {
	channel_name = arg;
	arg = "";
    }

    /* COMPATIBILITY: if there is no '_' in the name, prepend plyr_ */
    if ( member_array('_', channel_name) == -1 )
	channel_name = "plyr_" + channel_name;

    chan_type = channel_name[0..4] == "imud_";

    NCHANNEL_D->cmd_channel(channel_name, arg, chan_type);
}
