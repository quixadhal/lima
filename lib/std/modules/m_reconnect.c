/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** reconnect.c -- provide reconnection logic for socket users
**
** 95-May-11.  Deathblade.  Created.
*/

/*
** The client should fill in this function with the reconnection callback.
** The function prototype is:  void func(string key)
*/
static function reconn_func;

/*
** Store the retry time for each key as we try to reestablish contact.
** key -> ({ period, count }).  If a key has an entry here, then we are
** attempting to establish contact.
*/
static private mapping retry_info = ([ ]);

#define MAX_RETRY_PERIOD	3600	/* one hour */
#define RETRY_PER_PERIOD	4	/* 4 retries at each time period */

private nomask void time_to_reconnect(string key)
{
    /*
    ** If the connection was successful and the retry data was cleared,
    ** then just return.
    */
    if ( !retry_info[key] )
	return;

    evaluate(reconn_func, key);
}

static nomask void cancel_reconnection(string key)
{
    map_delete(retry_info, key);
}

static nomask void trigger_reconnect(string key)
{
    int delay;

    /*
    ** Attempt to reconnect to the target.  If we are not in the process
    ** of retrying, then set up the retry with a 1-minute period and begin
    ** immediately.
    **
    ** If we have been retrying and we've hit the max # retries for this
    ** time period, then double the time period and schedule a retry.
    */
    if ( !retry_info[key] )
    {
	retry_info[key] = ({ 60, 0 });
	delay = 0;
    }
    else if ( ++retry_info[key][1] == RETRY_PER_PERIOD )
    {
	delay = retry_info[key][0] * 2;
	if ( delay > MAX_RETRY_PERIOD )
	    delay = MAX_RETRY_PERIOD;
	retry_info[key] = ({ delay, 0 });
    }
    else
	delay = retry_info[key][0];

    call_out((: time_to_reconnect :), delay, key);
}


int stat_me()
{
    printf("\nRETRY INFO\n%O\n", retry_info);

    return 1;
}
