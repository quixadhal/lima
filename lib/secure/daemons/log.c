/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** log_d.c -- logging daemon
**
** 960102, Deathblade: created
*/

#include <log.h>
#include <dirs.h>

inherit DAEMON;

/*
** Map the symbolic log names to their respective log files.  Note that
** many names can map to a single file.
*/
private static mapping legal_logs = ([
    LOG_AUTODOC		: DIR_LOG "/AUTODOC",
    LOG_BANISH		: DIR_LOG "/banishes",
    LOG_BUG		: DIR_LOG "/BUGS",
    LOG_FORCE		: DIR_LOG "/forces",
    LOG_HELP_MISS	: DIR_LOG "/HELP_MISS",
    LOG_I3_ERROR	: DIR_LOG "/i3_errors",
    LOG_IDEA		: DIR_LOG "/IDEAS",
    LOG_LOGIN		: DIR_LOG "/logins",
    LOG_LOGIN_FAILURE	: DIR_LOG "/LOGIN_FAIL",
    LOG_QUEST		: DIR_LOG "/quests",
    LOG_QUIT		: DIR_LOG "/quits",
    LOG_SHUTDOWN	: DIR_LOG "/SHUTDOWN",
    LOG_TODO		: DIR_LOG "/TODOS",
    LOG_TYPO		: DIR_LOG "/TYPOS",
    LOG_WALL		: DIR_LOG "/walls",

    LOG_SNOOP		: DIR_SECURE_LOG "/snoops",
]);

void log(string which, string what)
{
    if ( !legal_logs[which] )
	error("illegal attempt to log to " + which + "\n");

    unguarded(1, (: write_file, legal_logs[which], what :));
}
