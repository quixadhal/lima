/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** listeners.c -- displays info on people listening to channels
**
** Note: this is an admin only command.
**
** 950727, Deathblade: Created.
*/

#include <mudlib.h>

inherit CMD;


private nomask string fmt_listener(object listener)
{
    if ( !listener )
	return "<null>";

    if ( function_exists("query_link", listener) )
    {
	if ( !listener->query_link() ||
	     !interactive(listener->query_link()) )
	    return sprintf("%s (link-dead)", listener->query_name());

	return sprintf("%s", listener->query_name());
    }

    return sprintf("%O", listener);
}

private void main()
{
    string * channels = NCHANNEL_D->query_channels();
    string channel_name;
    object * listeners;
    string output = "";

    if ( !check_privilege(1) )
    {
	write("Sorry, this command is only available to admins.\n");
	return;
    }

    foreach ( channel_name in channels )
    {
	string one_channel = "";

	listeners = NCHANNEL_D->query_listeners(channel_name) - ({ 0 });

	output += channel_name + ":\n" +
	    iwrap("    " +
		  implode(map_array(listeners, (: fmt_listener :)), ", ")) +
	    "\n\n";
    }

    this_user()->more(output);
}
