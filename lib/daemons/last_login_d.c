/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** last_login_d.c -- last login tracking daemon
**
** Manages tracking of the login times.  This is managed specially rather
** than with each user object so that we can review the information as a
** list without restoring a zillion objects.
**
//### no security has been implemented.  Is any needed?
**
** 950930, Ranma@Koko Wa: log players entering and quitting the game
** 950608, Deathblade:  created
*/

#include <security.h>
#include <log.h>

inherit M_DAEMON_DATA;

private mapping lastdata = ([ ]);

nomask void register_last(string userid, string addr)
{
    string s;

    if ( !addr && lastdata[userid] )
    {
        s = sprintf("%s leaves the game [%s]\n", userid, ctime(time()));
	LOG_D->log(LOG_QUIT, s);
	lastdata[userid][0] = time();
    }
    else
    {
        s = sprintf("%s enters the mud from %s [%s]\n", 
		    userid, addr, ctime(time()));
	LOG_D->log(LOG_LOGIN, s);

	lastdata[userid] = ({ time(), addr });
    }
    save_me();
}

nomask mixed * query_last(string userid)
{
    return lastdata[userid];
}

nomask mixed * query_times(int skip_sort)
{
    mixed * times = allocate(sizeof(lastdata));
    int i = 0;

    foreach ( string userid, mixed * info in lastdata )
    {
	times[i++] = ({ info[0], userid, info[1] });
    }

    if ( skip_sort )
	return times;

    return sort_array(times, 1);
}

nomask void remove_user(string userid, int skip_save)
{
    if ( !check_previous_privilege(1) )
	error("security: illegal attempt to remove user.\n");

    map_delete(lastdata, userid);

    if ( !skip_save )
	save_me();
}
