/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** chanlog_d.c
**
** Channel logging deamon.  Used to log a particular channel.  Typically
** used during a conference.
*/

#include <mudlib.h>
#include <security.h>
#include <log.h>

inherit M_ACCESS;

nomask void log_channel(string channel_name)
{
    object * listeners;

    listeners = NCHANNEL_D->query_listeners(channel_name);
    if ( listeners &&
         member_array(this_object(), listeners) != -1 )
        return;

    NCHANNEL_D->register_channels( ({ channel_name }) );
}
nomask void stop_logging()
{
    NCHANNEL_D->unregister_channels();
}

nomask void channel_rcv_string(string channel_name, string s)
{
    LOG_D->log(LOG_CHANNEL, s);
}
nomask void channel_rcv_soul(string channel_name, mixed * data)
{
    channel_rcv_string(channel_name, data[1][1]);
}

void create()
{
#ifdef CONF_IN_PROGRESS
    log_channel("plyr_conf");
#endif
    log_channel("imud_imud_gossip");
    log_channel("imud_imud_code");
}
