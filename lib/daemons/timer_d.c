/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** timer_d.c
**
** Timer facilities.
**
** 03-Feb-95. Deathblade. Created.
*/

#include <daemons.h>

/*
** This maps timer owners to an array of timer information.
*/
static private mapping timers;

void create()
{
    timers = ([ ]);
}

nomask void process_timer(object owner)
{
    mixed * data;
    string notice;
    int t;

    data = timers[owner];

    if ( data[1] == 0 )
        notice = "The timer has expired";
    else
        notice = sprintf("%d:%02d left on the timer", data[1]/60,data[1]%60);
    if ( data[3] )
        NCHANNEL_D->deliver_notice(data[3], notice);
    else
        tell_object(owner, notice + ".\n");

    if ( data[1] == 0 && data[2] )
    {
        data[1] = data[0];

        if ( data[4] && data[4] < data[0] )
            t = data[4];
        else
            t = data[0];
        notice = sprintf("Timer rescheduled for %d:%02d",
                         data[0]/60, data[0]%60);
        if ( data[3] )
            NCHANNEL_D->deliver_notice(data[3], notice);
        else
            tell_object(owner, notice + ".\n");
    }
    else if ( data[1] > 0 )
    {
        if ( data[1] > data[4] )
            t = data[4];
        else
            t = data[1];
    }

    if ( t )
    {
        data[1] -= t;
        call_out("process_timer", t, owner);
    }
}

varargs nomask string add_timer(int delay,		/* timer delay */
			     int repeating,	/* repeating timer? */
			     string channel,	/* channel for notifies */
			     int notify,	/* period for notifies */
			     object owner	/* if not this_user() */
			     )
{
    int t;

    if ( !delay || notify > delay )
	return "Bad parameters.\n";
    if ( !owner )
        owner = this_user();
    if ( !owner )
        return "No owner.\n";

    /*
    ** Compute the first delay time
    */
    if ( notify && notify < delay )
	t = notify;
    else
        t = delay;

    timers[owner] = ({ delay, delay - t, repeating, channel, notify });
    remove_call_out("process_timer");
    call_out("process_timer", t, owner);

    if ( channel )
    {
        string notice;

        notice = sprintf("timer set to %d:%02d", delay/60, delay%60);
        NCHANNEL_D->deliver_notice(channel, notice);
    }
    return "Done.\n";
}
mapping query_timers() { return timers; }
