/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** timer_d.c
**
** Timer facilities.
**
** 03-Feb-95. Deathblade. Created.
*/

#include <daemons.h>

class timer_info
{
    int		delay;		/* delay between start and end */
    int		time_left;	/* after current call_out(), how much longer */
    int		repeating;	/* is it a repeating timer? */
    string	channel_name;	/* channel to announce over */
    int		notify_period;	/* how often to give timer notifications */
}

/*
** This maps timer owners to a timer_info structure.
*/
static private mapping timers;

void create()
{
    timers = ([ ]);
}

nomask void process_timer(object owner)
{
    class timer_info data;
    string notice;
    int t;

    data = timers[owner];

    if ( data->time_left == 0 )
        notice = "The timer has expired";
    else
        notice = sprintf("%d:%02d left on the timer",
			 data->time_left / 60, data->time_left % 60);
    if ( data->channel_name )
        NCHANNEL_D->deliver_notice(data->channel_name, notice);
    else
        tell_object(owner, notice + ".\n");

    if ( data->time_left == 0 && data->repeating )
    {
        data->time_left = data->delay;

        if ( data->notify_period && data->notify_period < data->delay )
            t = data->notify_period;
        else
            t = data->delay;
        notice = sprintf("Timer rescheduled for %d:%02d",
                         data->delay / 60, data->delay % 60);
        if ( data->channel_name )
            NCHANNEL_D->deliver_notice(data->channel_name, notice);
        else
            tell_object(owner, notice + ".\n");
    }
    else if ( data->time_left > 0 )
    {
        if ( data->time_left > data->notify_period )
            t = data->notify_period;
        else
            t = data->time_left;
    }

    if ( t )
    {
        data->time_left -= t;
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
    class timer_info info;
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

    info = new(class timer_info);
    info->delay		= delay;
    info->time_left	= delay - t;
    info->repeating	= repeating;
    info->channel_name	= channel;
    info->notify_period	= notify;
    timers[owner] = info;

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

mapping query_timers()
{
    return timers;
}
