/* Do not remove the headers from this file! see /USAGE for more info. */

//:ADMINCOMMAND
//USAGE: listeners
//       listeners <channel name>
//
//Displays list of people listening to channels
//Admin only

/*
** listeners.c -- displays info on people listening to channels
**
** Note: this is an admin only command.
**
** 950727, Deathblade: Created.
*/

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

private void show_one_channel(string channel_name)
{
  object * listeners;
  int flags;
  string flagstr = ":";

  listeners = CHANNEL_D->query_listeners(channel_name);
  if ( !listeners )
  {
    out("No such channel: " + channel_name + "\n");
    return;
  }
  listeners -= ({ 0 });

  flags = CHANNEL_D->query_flags(channel_name);

  if ( flags & CHANNEL_WIZ_ONLY )
    flagstr += " wiz";
  if ( flags & CHANNEL_ADMIN_ONLY )
    flagstr += " admin";
  if ( flags & CHANNEL_PERMANENT )
    flagstr += " permanent";

  out(channel_name + flagstr + "\n    " +
      implode(map_array(listeners, (: fmt_listener :)), ", ") +
      "\n\n");
}

private void show_users_channels(object user)
{
  out("*** NOT YET IMPLEMENTED ***\n");
}

private void main(mixed *argv)
{
  if ( !check_privilege(1) )
  {
    out("Sorry, this command is only available to admins.\n");
    return;
  }

  if ( argv[0] )
  {
    if ( objectp(argv[0]) )
      show_users_channels(argv[0]);
    else
      show_one_channel(argv[0]);
  } else {
    map(CHANNEL_D->query_channels(), (: show_one_channel :));
  }
}
