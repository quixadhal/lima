/* Do not remove the headers from this file! see /USAGE for more info. */


/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** shutdown_d.c -- shuts down the mud in a timed fashion, displaying a
**                 message each minute of the countdown.
**
** the "type" integer that propogates down through the functions determines
** if the mud is to be shutdown or rebooted.  This basically means pass
** one number or another to the shutdown() efun for the controlling script
** to catch.
**
** 960612, Alexus: created
*/

#include <mudlib.h>
#include <log.h>

inherit DAEMON;

int shutdown_status; // -1 = no reboot in progress, # = # of mins remaining

private void count_down(int, string);

int query_shutdown_status() { return shutdown_status; }

int cancel_shutdown()
{
    if(!check_previous_privilege(1)) return -1;

    shutdown_status = -1;
    return remove_call_out("count_down");
}

void start_shutdown(int num, string s)
{
    int i, size;

    if(!check_previous_privilege(1)) return;
    count_down(num, s);
    shutdown_status = num;
}

private void do_it(string s)
{
    LOG_D->log(LOG_SHUTDOWN,
      sprintf("SHUTDOWN (%s) by shutdown_d [%s]\n",
        ctime(time()), s));
 
    unguarded(1, (: shutdown :));
}

private void count_down(int num, string s)
{
    if(!num)
    {
	tell(users(), "Gamedriver tells you: Game shutting down immediately!\n");
	call_out("do_it", 0, s);
	return;
    }
    tell(users(), "Gamedriver tells you: Shutdown in " + num + " minutes.\n");
    num--;
    call_out("count_down", 60, num, s);
}

