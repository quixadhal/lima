/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** listeners.c -- displays info on people listening to channels
**
** Note: this is an admin only command.
**
** 950727, Deathblade: Created.
*/

#include <mudlib.h>
#include <channel.h>

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
    string * channels = CHANNEL_D->query_channels();
    mapping permanent = CHANNEL_D->query_permanent();
    string channel_name;
    object * listeners;

    if ( !check_privilege(1) )
    {
	out("Sorry, this command is only available to admins.\n");
	return;
    }

    foreach ( channel_name in channels )
    {
	string one_channel = "";
	int flags = permanent[channel_name];
	string flagstr = ":\n";

	listeners = CHANNEL_D->query_listeners(channel_name) - ({ 0 });

	if ( !undefinedp(flags) )
	{
	    if ( flags & CHANNEL_WIZ_ONLY )
		flagstr = ": wiz permanent\n";
	    else if ( flags & CHANNEL_ADMIN_ONLY )
		flagstr = ": admin permanent\n";
	    else
		flagstr = ": permanent\n";
	}
	    
	out( channel_name + flagstr + "    " +
		  implode(map_array(listeners, (: fmt_listener :)), ", ") +
	    "\n\n");
    }

}


