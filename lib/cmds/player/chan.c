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

inherit DAEMON;

int main(string arg)
{
    string channel_name;

    if ( !arg || arg == "" || (wizardp(this_user()) && arg == "-d") )
    {
	string * channel_list;

	channel_list = this_body()->query_channel_list();
	if ( sizeof(channel_list) == 0 )
	    write("You are not listening to any channels.\n");
	else
	{
	    if ( arg != "-d" )
		channel_list = map_array(channel_list, (: explode($1, "_")[<1] :));
					    
	    write("You are listening to: " +
		  implode(channel_list, ", ") + ".\n");
	}
				     
	return 1;
    }

    if ( sscanf(arg, "%s %s", channel_name, arg) != 2 )
    {
	channel_name = arg;
	arg = "";
    }
	    
    /* all channels thru this ob are plyr_XXX */
    channel_name = "plyr_" + channel_name;
	    
    NCHANNEL_D->cmd_channel(channel_name, arg);
	    
    return 1;
}
