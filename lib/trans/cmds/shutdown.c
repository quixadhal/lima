/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**  /trans/cmds/shutdown - shuts the game down imediately or starts the
**                         shutdown daemon.
**
**  960614 - Alexus adds timed shutdown daemon support
*/
#include <log.h>

inherit CMD;

private nomask void do_shutdown()
{
    unguarded(1, (: shutdown :));
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
      num = SHUTDOWN_D->query_shutdown_status();
      if(!num) {
        write("There is no shutdown in progress.\n");
        return;
      }
      SHUTDOWN_D->cancel_shutdown();
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
      SHUTDOWN_D->start_shutdown(num, s);
      write("Shutdown started.\n");
      return;
    }
    LOG_D->log(LOG_SHUTDOWN,
      sprintf("SHUTDOWN at %s by %s [%s]\n",
        ctime(time()), this_body()->query_name(), s));
      tell(users(), "Gamedriver tells you: Game shutting down immediately!\n");
      users()->quit();
      call_out((: do_shutdown :), 0);
}

