/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** timer.c
**
** Simple interface to the timer daemon
**
** 04-Feb-95. Deathblade. Created.
*/

#include <mudlib.h>
inherit CMD;

private void main(mixed * args, mapping flags)
{
    string channel_name;

    channel_name = args[1];
    if ( channel_name )
    {
	string user_name = NCHANNEL_D->user_channel_name(channel_name);
	if ( channel_name == user_name )
	    channel_name = "plyr_" + channel_name;
    }

    out(TIMER_D->add_timer(args[0], flags["r"], channel_name, args[2]));
}
