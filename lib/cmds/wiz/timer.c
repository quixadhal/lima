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
    if ( channel_name && sizeof(explode(channel_name, "_")) == 1 )
        channel_name = "plyr_" + channel_name;

    write(TIMER_D->add_timer(args[0],flags["r"],channel_name,args[2]));
}
