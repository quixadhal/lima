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

inherit M_ACCESS;

#define SAVE_FILE		"/data/daemons/last_login_d"

private mapping lastdata = ([ ]);

void create()
{
    set_privilege(1);
    restore_object(SAVE_FILE);
}

varargs nomask void register_last(string userid, string addr)
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
    unguarded(1, (: save_object, SAVE_FILE :));
}

nomask mixed * query_last(string userid)
{
    return lastdata[userid];
}
