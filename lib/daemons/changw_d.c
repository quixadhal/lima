/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** changw_d.c
**
** Channel gateway for private mud-to-mud channels.  This uses the
** remote_d daemon to communicate with the remote muds, and the
** channel system for the bulk of the channel work.
**
** 14-Apr-95. Deathblade.  Created.
*/

#include <mudlib.h>
#include <daemons.h>
#include <security.h>

inherit M_ACCESS;
inherit M_REMOTE;

#define SAVE_FILE "/data/daemons/changw_d"

DOC_MODULE("This daemon acts as a gateway between local channels and remote-mud channels.");

private string * gatewayed_channels = ({ "wiz_s.wiz" });

private static int filter_msg = 0;


void create()
{
    set_privilege(1);
    restore_object(SAVE_FILE);
    NCHANNEL_D->register_channels(gatewayed_channels);
}

void channel_rcv_string(string channel_name, string message)
{
    if ( !filter_msg )
    {
	unguarded(1, (: call_broadcast,
		      CHANGW_D,
		      "remote_rcv_string",
		      ({ channel_name, message }) :));
    }
    filter_msg = 0;
}

void channel_rcv_soul(string channel_name, mixed * data)
{
    if ( !filter_msg )
    {
	data = ({ data[0]->query_real_name(), data[1] });

	unguarded(1, (: call_broadcast,
		      CHANGW_D,
		      "remote_rcv_soul",
		      ({ channel_name, data }) :));
    }
    filter_msg = 0;
}

void remote_rcv_string(string channel_name, string message)
{
    filter_msg = 1;
    NCHANNEL_D->deliver_string(channel_name, message);
}

void remote_rcv_soul(string channel_name, mixed * data)
{
    data = ({ map_array(data[0], (: find_body :)) - ({ 0 }),
		  data[1] });

    filter_msg = 1;
    NCHANNEL_D->deliver_raw_soul(channel_name, data);
}

void channel_add(string which_channel)
{
    gatewayed_channels += ({ which_channel });
    unguarded(1, (: save_object, SAVE_FILE :));

    NCHANNEL_D->register_channels(({ which_channel }));
}
void channel_remove(string which_channel)
{
    gatewayed_channels -= ({ which_channel });
    unguarded(1, (: save_object, SAVE_FILE :));

    NCHANNEL_D->unregister_channels(({ which_channel }));
}
string * query_gatewayed_channels()
{
    return gatewayed_channels;
}
