/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** logger_d.c
**
** Logging deamon.  Used to write various logs for "unsecure"
** objects.
*/

#include <mudlib.h>
#include <security.h>

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
    NCHANNEL_D->unregister_all();
}

nomask void channel_rcv_string(string channel_name, string s)
{
    unguarded(1, (: write_file, "/log/logger", s :));
}
nomask void channel_rcv_soul(string channel_name, mixed * data)
{
    channel_rcv_string(channel_name, data[1][1]);
}

#ifdef CONF_IN_PROGRESS
void create() { log_channel("plyr_conf"); }
#endif
