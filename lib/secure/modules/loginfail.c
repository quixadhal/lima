/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** loginfail.c -- record/manage login/su failure information
**
** 10-Jun-95.  Deathblade.  Created.
*/

varargs mixed unguarded(mixed priv, function code);

string query_userid();
void save_me();
varargs void restore_me(string some_name);

#define FAILURE_LOG_FILE	"/log/LOGIN_FAIL"

private mixed *	failures = ({ });
private int notify_time;	/* time user notified of bad logins */


static nomask void register_failure(string addr)
{
    string s;

    if(!arrayp(failures))
      failures = ({});
    failures += ({ ({ time(), addr }) });
    save_me();

    s = sprintf("%s: %s from %s\n", query_userid(), ctime(time()), addr);
    unguarded(1, (: write_file, FAILURE_LOG_FILE, s :));
}

nomask mixed * query_failures()
{
    return copy(failures);
}

nomask void clear_failures()
{
#ifdef NEED_UNRESTRICTED_PLAYER_CMD
    if ( !check_privilege(query_userid()) )
#endif
//    if ( this_user() != this_object() )		/* ### for now */
    if ( this_user()->query_link() != this_object() )	/* ### for now */
    {
	error("* Security violation: you cannot clear this info\n");
    }

    failures = ({ });
    save_me();
}

static nomask void report_login_failures()
{
    mixed * failures = query_failures();
    int count;

    if ( !sizeof(failures) )
	return;

    count = sizeof(filter_array(failures, (: $1[0] > $(notify_time) :)));
    if ( count )
    {
	/* ### hmm... this count is total, not since last login */
	printf("You had %d failed login attempt(s) since your last login.\n",
	       count);
    }

    notify_time = time();
}
