/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** log_d.c -- logging daemon
**
** 960102, Deathblade: created
*/

#include <log.h>
#include <dirs.h>

inherit M_ACCESS;

/*
** Map the symbolic log names to their respective log files.  Note that
** many names can map to a single file.
*/
private static mapping legal_logs = ([
    LOG_AUTODOC		: DIR_LOG "/AUTODOC",
    LOG_BANISH		: DIR_LOG "/banishes",
    LOG_BUG		: DIR_LOG "/BUGS",
    LOG_CHANNEL		: DIR_LOG "/chanlog",
    LOG_FORCE		: DIR_LOG "/forces",
    LOG_FTP		: DIR_LOG "/ftp",
    LOG_HELP_MISS	: DIR_LOG "/HELP_MISS",
    LOG_I3_ERROR	: DIR_LOG "/i3_errors",
    LOG_IDEA		: DIR_LOG "/IDEAS",
    LOG_LOGIN		: DIR_LOG "/logins",
    LOG_LOGIN_FAILURE	: DIR_LOG "/LOGIN_FAIL",
    LOG_MAIL_ERROR	: DIR_LOG "/mail_errs",
    LOG_QUEST		: DIR_LOG "/quests",
    LOG_QUIT            : DIR_LOG "/logins",
    LOG_SHUTDOWN	: DIR_LOG "/SHUTDOWN",
    LOG_SNOOP		: DIR_SECURE_LOG "/snoops",
    LOG_SOCKET		: DIR_LOG "/socket",
    LOG_TODO		: DIR_LOG "/TODOS",
    LOG_TYPO		: DIR_LOG "/TYPOS",
    LOG_WALL		: DIR_LOG "/walls",
    LOG_FTP		: DIR_LOG "/ftp",
]);

private static string * timestamps = ({
    LOG_CHANNEL,
});

void create() {
    set_privilege(1);
}

void log(string which, string what)
{
    if ( !legal_logs[which] )
	error("illegal attempt to log to " + which + "\n");

    if ( member_array(which, timestamps) != -1 )
	what = ctime(time()) + ": " + what;

    unguarded(1, (: write_file, legal_logs[which], what :));
}
