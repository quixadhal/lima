/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**  /trans/cmds/shutdown - shuts the game down imediately or starts the
**                         shutdown daemon.
**
**  960614 - Alexus adds timed shutdown daemon support
*/
#include <log.h>

inherit CMD;

static void do_shutdown(string s)
{
    if (!check_privilege(1)) {
	LOG_D->log(LOG_SHUTDOWN,
		   sprintf("do_shutdown() failed due to insufficient privilege (possibly due to being called directly) at %s\n", ctime(time())));
	return;
    }
    
    LOG_D->log(LOG_SHUTDOWN,
      sprintf("SHUTDOWN executed at %s [%s]\n",
        ctime(time()), s));

    foreach (object ob in users())
	catch(ob->quit());
 
    unguarded(1, (: shutdown :));
}

private void count_down(int num, string s) {
    if(!num)
    {
	tell(users(), "Gamedriver tells you: Game shutting down immediately!\n");
	do_shutdown(s);
	return;
    }
    tell(users(), "Gamedriver tells you: Shutdown in " + M_GRAMMAR->number_of(num, "minute") + ".\n");
    num--;
    call_out("count_down", 60, num, s);
}

private void main(mixed *args, mapping flags)
{
    int num;
    string s;

    if ( !check_privilege(1) )
    {
	outf("You must think your an Administrator, don't you?\n");
	return;
    }

    s = implode(args, " ");

    // handle cancelation checking first!
    if(flags["c"]) {
	if (remove_call_out("do_shutdown") == -1) {
	    write("There is no shutdown in progress.\n");
	    return;
	}
	LOG_D->log(LOG_SHUTDOWN,
		   sprintf( "Shutdown cancelled %s by %s [%s]\n",
			    ctime(time()), this_body()->query_name(), s));
	tell(users(), "Gamedriver tells you: Shutdown has been cancelled.\n");
	return;
    }

    // Check to see if a -t # switch was given.  If so, don't shut down,
    // just start the shutdown daemon.
    if(flags["t"]) {
        LOG_D->log(LOG_SHUTDOWN,
		   sprintf("Shutdown started %s by %s [%s]\n",
			   ctime(time()), this_body()->query_name(), s));
	num = to_int(flags["t"]);
	count_down(num, s);
	write("Shutdown started.\n");
	return;
    }
    LOG_D->log(LOG_SHUTDOWN,
	       sprintf("Immediate shutdown requested by %s\n",
		       this_user()->query_userid()));
    tell(users(), "Gamedriver tells you: Game shutting down immediately!\n");
    do_shutdown(s);
}

