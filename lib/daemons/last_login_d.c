/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** last_login_d.c -- last login tracking daemon
**
** Manages tracking of the login times.  This is managed specially rather
** than with each user object so that we can review the information as a
** list without restoring a zillion objects.
**
** ### no security has been implemented.  Is any needed?
**
** 08-Jun-95.  Deathblade.  Created.
*/

#include <security.h>

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
    if ( !addr && lastdata[userid] )
    {
	lastdata[userid][0] = time();
    }
    else
    {
	lastdata[userid] = ({ time(), addr });
    }

    unguarded(1, (: save_object, SAVE_FILE :));
}

nomask mixed * query_last(string userid)
{
    return lastdata[userid];
}
