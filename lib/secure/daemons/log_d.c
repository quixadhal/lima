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
private nosave mapping legal_logs = ([
    LOG_ADDEMOTE        : DIR_LOG "/addemote",
    LOG_AUTODOC		: DIR_LOG "/autodoc",
    LOG_BANISH		: DIR_LOG "/banishes",
    LOG_BUG		: DIR_LOG "/bugs",
    LOG_CHANNEL		: DIR_LOG "/chanlog",
    LOG_FORCE		: DIR_LOG "/forces",
    LOG_FTP		: DIR_LOG "/ftp",
    LOG_HELP_MISS	: DIR_LOG "/help_miss",
    LOG_I3_ERROR	: DIR_LOG "/i3_errors",
    LOG_IDEA		: DIR_LOG "/ideas",
    LOG_LOGIN		: DIR_LOG "/logins",
    LOG_LOGIN_FAILURE	: DIR_LOG "/login_fail",
    LOG_MAIL_ERROR	: DIR_LOG "/mail_errs",
    LOG_NEW_PLAYERS     : DIR_LOG "/new_players",
    LOG_QUEST		: DIR_LOG "/quests",
    LOG_QUIT            : DIR_LOG "/logins",
    LOG_SETNAME         : DIR_LOG "/setname",
    LOG_SHUTDOWN	: DIR_LOG "/shutdown",
    LOG_SNOOP		: DIR_SECURE_LOG "/snoops",
    LOG_SOCKET		: DIR_LOG "/socket",
    LOG_TODO		: DIR_LOG "/todos",
    LOG_TYPO		: DIR_LOG "/typos",
    LOG_WALL		: DIR_LOG "/walls",
    LOG_FTP		: DIR_LOG "/ftp",
    LOG_RCP             : DIR_LOG "/rcp",
]);

private nosave string * timestamps = ({
    LOG_BANISH,
    LOG_CHANNEL,
    LOG_SHUTDOWN,
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
