/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** last.c -- show last login information
**
** 960528, Deathblade: created.
*/

inherit CMD;

#define SECS_PER_DAY	(24 * 60 * 60)


private void main(mixed *arg, mapping flags)
{
    int minimum;
    int maximum;
    int count;
    mixed * times;
    int i;

    if ( sizeof(arg[0]) )
    {
	foreach ( string user in arg[0] )
	{
	    mixed * last;

	    user = lower_case(user);

	    last = LAST_LOGIN_D->query_last(user);
	    if ( !last )
		outf("No information for: %s\n", user);
	    else
	    {
		int is_on = find_user(user) != 0;
		outf(is_on ? "%s logged on at %s from %s\n" :
		             "%s logged out at %s from %s\n",
		     user, ctime(last[0]), last[1]);
	    }
	}
	return;
    }

    minimum = 0;
    maximum = time();
    count = to_int(flags["n"]);

    if ( flags["d"] )
	minimum = time() - (to_int(flags["d"]) * SECS_PER_DAY);
    if ( flags["D"] )
	maximum = time() - (to_int(flags["D"]) * SECS_PER_DAY);

    if ( minimum >= maximum )
    {
	outf("ERROR: invalid range.\n");
	return;
    }

    times = filter(LAST_LOGIN_D->query_times(),
		   (: $1[0] >= $(minimum) && $1[0] <= $(maximum) :));

    if ( !flags["s"] )
	outf("%d users. %s to %s.\n%s\n",
	     sizeof(times), ctime(minimum), ctime(maximum),
	     repeat_string("-", 75));

    if ( !flags["s"] && count && sizeof(times) > count )
	outf("... skipping %d users.\n", sizeof(times) - count);

    i = count && (sizeof(times) - count);
    if ( i < 0 )
	i = 0;
    for ( ; i < sizeof(times); ++i )
    {
	int is_on = find_user(times[i][1]) != 0;

	outf(is_on ? "%s logged on at %s from %s\n" :
		     "%s logged out at %s from %s\n",
	      times[i][1], ctime(times[i][0]), times[i][2]);
    }
}
