/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define mud_name()		MUD_NAME
/* You don't have permission to change the next 2 lines w/o our consent.*/
#define lib_name()		"Lima"
#define lib_version()		"0.9r2 (pre-alpha)"
#define lib_status()		"Open for the public"
#define driver_version()	__VERSION__
#define __HOST__		"lima.imaginary.com"

#define START			"/domains/std/wizroom"

#define HISTORY_CHAR	'%' /* ! will give you problems since it's used as input escape. */
#define AUTOMATIC_REHASH

#define ADMIN_EMAIL	"rust@virginia.edu or gstein@svpal.org"

#undef NO_NEW_PLAYERS
#define OBVIOUS_EXITS   

#define USE_GAME_FEATURES
#ifdef USE_GAME_FEATURES
#define USE_STATS
#ifdef USE_STATS
#define USE_COMBAT
#endif
#define USE_GUILDS
#endif


// Auto wiz means guest wizard only.  You still have to use the admtool
// command to make the person a full wizard w/ a directory.
#define AUTO_WIZ

#define BUG_NEWSGROUP "lima.bugs"
#define TYPO_NEWSGROUP "reporter.typos"
#define IDEA_NEWSGROUP "lima.ideas"

// Define this if you want bug, typo and idea commands to log to /log instead
// of posting news
#undef LOG_DONT_POST

#define BUG_MSG \
"You have just encountered a bug.  Please use the bug command to report it.\n"


// I'd recommend not changing anything below this point.
#define LINK_PATH(x)		sprintf("/data/links/%c/%s",x[0],x)
#define USER_PATH(x)		sprintf("/data/players/%c/%s",x[0],x)
#define PRELOAD_FILE		"/data/config/preload"
#undef RCS_SERVER		"quendor"

#define WIZ_DIR			"/wiz"
#define LOG_DIR			"/log"

#define WELCOME_FILE	"/data/config/WELCOME"
#define NEW_PLAYER	"/data/config/NEW_PLAYER"
#define MOTD_FILE	"/data/config/MOTD"

#endif /* __CONFIG_H__ */
