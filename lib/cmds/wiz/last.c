/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** last.c -- show last login information
**
** 960528, Deathblade: created.
*/

//:COMMAND
//USAGE: last [-s] [-n count] [-d days] [-D days] [user1 user2 ...]
//
//  -s       : be "silent" -- trim headers, displaying just the data
//  -n count : only display this many users (the most recent)
//  -d days  : display users logged in WITHIN this many days
//  -D days  : display users logged in OLDER than this many days
//
//  user...  : display login information for these users
//
//Note that the options can be combined, but you'll get an error if you
//use -d and -D to, say, ask for all users logged in during the past 30
//days and those logged in before 60 days ago.
//
//For each user found, their name is displayed followed by when they
//logged in (if they are still on) or when they logged out, followed by
//where they connected from.

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
