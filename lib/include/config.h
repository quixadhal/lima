/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __CONFIG_H__
#define __CONFIG_H__

// ------- options which have been moved to other files:
#include "config_combat.h"
// -----------------------------------------------------------

/* changes these as you please */
#define lib_status()		"game development"
#define __HOST__		"host.name"
#define ADMIN_EMAIL		"user@host.name"

#undef HANDLE_MANY_WIZARDS	/* if your mud has oodles of wizards... */

#define START			"/domains/std/rooms/beach/sandy_beach"
#define WIZARD_START		"/domains/std/wizroom"

#define LOGIN_PROMPT		"Your name? "

/* ! will give you problems since it's used as input escape. */
#define HISTORY_CHAR		'%'

#define EVERYTHING_SAVES		/* Note: if you define this, you have to
				   modify the lib to save the correct vars */

#define OBVIOUS_EXITS		/* show obvious exits when looking (top) */
#undef OBVIOUS_EXITS_BOTTOM	/* show obvious exits at bottom of long() */

#define DEFAULT_LIGHT_LEVEL	1	/* a room's default light */

#define EVERYONE_HAS_A_PLAN

#define PLAYERS_START_WHERE_THEY_QUIT

#define USE_WIZ_POSITION	/* use the "position" scheme for wizzes */
#define USE_QUESTS		/* quests are not always related to gaming,
				   so we have a separate config flag */
#define USE_STATUS_LINE            /* Allow use of Infocom-like Status Line */
#define USE_GAME_FEATURES	/* use a lot of features related to gaming */

#ifdef USE_GAME_FEATURES	/* pull in a set for gaming features */
# define USE_RACES
# define USE_STATS
# define USE_GUILDS
# define USE_SKILLS
# define USE_TITLES
# define USE_BODYSLOTS
//# define USE_SIMPLE_LEVEL	// use the simple level number scheme for players
#endif

/* one or the other should be defined */
#define USE_SIZE

#ifndef USE_SIZE  // Mass and Size do the same thing, so you can only use 1
# define USE_MASS
#endif

#undef NO_NEW_PLAYERS			/* disallow character creation */
#define LOGIN_NAME_WAIT		300	/* seconds to wait for a name */
#define LOGIN_PASSWORD_WAIT	180	/* seconds for the password */

// Auto wiz means guest wizard only.  You still have to use the admtool
// command to make the person a full wizard w/ a directory.
#define AUTO_WIZ

// Define this if you want bug, typo and idea commands to log to /log instead
// of posting news
#undef LOG_DONT_POST

// If this is defined, then each player can set their own colours individually.
// There is some disk space/memory overhead for keeping track of all this,
// though.
#define CONFIGURABLE_COLOUR

#define BUG_NEWSGROUP "reporter.bugs"
#define TYPO_NEWSGROUP "reporter.typos"
#define IDEA_NEWSGROUP "reporter.ideas"
#define TODO_NEWSGROUP "reporter.todo"
#define QUESTION_NEWSGROUP "reporter.question"
#define FEEDBACK_NEWSGROUP "lima.discussion"


#define BUG_MSG \
"You have just encountered a bug.  Please use the bug command to report it.\n"

// Define this next one to limit how many aliases a person can have.
#undef MAX_NUMBER_OF_ALIASES	30

// max number of entries in the channel history (the /last command)
#define CHANNEL_HISTORY_SIZE	20

// undef it to not allow anonymous ftp.  Anon ftp users are limited to /ftp/pub.
#define ALLOW_ANON_FTP

// Whether anon ftp users can *put* files.  Only works if ALLOW_ANON_FTP is
// defined.

#undef ANON_CAN_PUT


// I'd recommend not changing anything below this point.
#define LINK_PATH(x)		sprintf("/data/links/%c/%s",x[0],x)
#define USER_PATH(x)		sprintf("/data/players/%c/%s",x[0],x)
#define PSHELL_PATH(x)		sprintf("/data/pshells/%c/%s",x[0],x)

#ifdef HANDLE_MANY_WIZARDS
#define WSHELL_PATH(x)		sprintf("/data/wshells/%c/%s",x[0],x)
#else
#define WSHELL_PATH(x)		sprintf("/data/wshells/%s",x)
#endif

#define PRELOAD_FILE		"/data/config/preload"

#define WIZ_DIR			"/wiz"

#define WELCOME_FILE		"/data/config/WELCOME"
#define NEW_PLAYER		"/data/config/NEW_PLAYER"
#define MOTD_FILE		"/data/config/MOTD"

#endif /* __CONFIG_H__ */
