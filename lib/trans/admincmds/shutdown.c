/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**  /trans/cmds/shutdown - shuts the game down imediately or starts the
**                         shutdown daemon.
**
**  960614 - Alexus adds timed shutdown daemon support
**  961215, Deathblade: removed time stamping (let LOG_D do it)
*/

//:ADMINCOMMAND
//USAGE: shutdown -t<time> <reason>
//       shutdown -c <reason>
//
//Shuts down the mud.
//Time specified (with -t flag) is delay in minutes to shutdown.
//Use the -c flag to cancel a shutdown
//
//Admin only

#include <log.h>

#define THE_BIG_GUY	"Gamedriver"
#define tell_all(s)	tell(users(), (s))

inherit CMD;

int call_number;

private void do_shutdown(string s)
{
    if ( !check_privilege(1) )
    {
	LOG_D->log(LOG_SHUTDOWN,
		   "do_shutdown() failed: insufficient privilege\n");
	return;
    }

    LOG_D->log(LOG_SHUTDOWN, sprintf("SHUTDOWN performed [%s]\n", s));

    foreach ( object ob in users() )
	catch(ob->quit());

    unguarded(1, (: shutdown :));
}

private void count_down(int num, string s)
{
    if ( !num )
    {
	tell_all(THE_BIG_GUY " tells you: Game shutting down immediately!\n");
	do_shutdown(s);
	return;
    }

    tell_all(THE_BIG_GUY " tells you: Shutdown in " + M_GRAMMAR->number_of(num, "minute") + ".\n");
    num--;
    call_number=call_out((: count_down, num, s :), 60);
}

private void main(mixed *args, mapping flags)
{
    int num;
    string s;

    if ( !check_privilege(1) )
    {
	out("You must think you're an Administrator, don't you?\n");
	return;
    }

    s = implode(args[0], " ");

    // handle cancelation checking first!
    if(flags["c"]) {
      if (remove_call_out(call_number) == -1) {
	    out("There is no shutdown in progress.\n");
	    return;
	}
	LOG_D->log(LOG_SHUTDOWN,
		   sprintf("Shutdown cancelled by %s [%s]\n",
			    this_body()->query_name(), s));
	tell_all(THE_BIG_GUY " tells you: Shutdown has been cancelled.\n");
          call_number=0;
	return;
    }
    /* Check to make sure that a shutdown isn't already in progress */
    if(call_number)
      {
	out("Shutdown is already in progress.\n");
	call_number=0;
	return;
      }

    // Check to see if a -t # switch was given.  If so, don't shut down,
    // just start the shutdown daemon.
    if(flags["t"]) {
        LOG_D->log(LOG_SHUTDOWN,
		   sprintf("Shutdown started by %s [%s]\n",
			   this_body()->query_name(), s));
	num = to_int(flags["t"]);
	count_down(num, s);
	out("Shutdown started.\n");
	return;
    }

    LOG_D->log(LOG_SHUTDOWN,
	       sprintf("Immediate shutdown requested by %s\n",
		       this_user()->query_userid()));
    tell_all(THE_BIG_GUY " tells you: Game shutting down immediately!\n");
    do_shutdown(s);
}

// Used by the out of memory code; could be added elsewhere too
void automatic_shutdown(string reason) {
    if (!check_privilege(1))
        error("automatic_shutdown() failed: Permission denied\n");

    tell_all(THE_BIG_GUY " shouts: Automatic shutdown (" + reason + ") triggered.\n");
    LOG_D->log(LOG_SHUTDOWN, "Shutdown started by " THE_BIG_GUY "[" + reason + "]\n");
    
    count_down(10, reason);
}