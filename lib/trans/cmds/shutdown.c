/* Do not remove the headers from this file! see /USAGE for more info. */

#include <log.h>

inherit CMD;


private nomask void do_shutdown()
{
    unguarded(1, (: shutdown :));
}

private void main(string s)
{
    if ( !check_privilege(1) )
    {
	outf("Only admins may shut down the game.\n");
	return;
    }

    switch (s)
    {
    case 0:
    case "":
	out("You must give a reason to shut the game down.\n");
	return;

    default:
	if (s[0..3] == "now") {
	    out("Now is not a reason.\n");
	    return;
	}
	if (strsrch(s, "fuck") != -1) {
	    out("Ask nicely.\n");
	    return;
	}
	LOG_D->log(LOG_SHUTDOWN,
		   sprintf("SHUTDOWN (%s) by %s [%s]\n",
			   ctime(time()), this_body()->query_name(), s));
	shout("LPmud shut down by "+this_body()->query_name()+".\n");
	users()->quit();
	call_out((: do_shutdown :), 0);
    }
}


