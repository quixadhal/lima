/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __CONFIG_H__
#define __CONFIG_H__

// ------- options which have been moved to other files:
#include "config_combat.h"
// -----------------------------------------------------------

/* changes these as you please */
#define lib_status()		"mudlib development"
#define __HOST__		"lima.imaginary.com"
#define ADMIN_EMAIL		"user@host.name"

#define START			"/domains/std/rooms/beach/sandy_beach"
#define WIZARD_START		"/domains/std/wizroom"

#define LOGIN_PROMPT		"Your name? "

#define HISTORY_CHAR	'%' /* ! will give you problems since it's used as input escape. */
#define AUTOMATIC_REHASH

#define OBVIOUS_EXITS

#define EVERYONE_HAS_A_PLAN


#define USE_GAME_FEATURES

#ifdef USE_GAME_FEATURES
# define USE_STATS
# define USE_GUILDS
# define USE_SKILLS
# define USE_TITLES
# define USE_SIZE
# ifndef USE_SIZE  // Mass and Size do the same thing, so you can only use 1
#  define USE_MASS
# endif
//# define USE_SIMPLE_LEVEL	// use the simple level number scheme for players
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

#define BUG_NEWSGROUP "lima.bugs"
#define TYPO_NEWSGROUP "reporter.typos"
#define IDEA_NEWSGROUP "lima.ideas"
#define TODO_NEWSGROUP "lima.todo"


#define BUG_MSG \
"You have just encountered a bug.  Please use the bug command to report it.\n"

// Define this next one to limit how many aliases a person can have.
#undef MAX_NUMBER_OF_ALIASES	30


// I'd recommend not changing anything below this point.
#define LINK_PATH(x)		sprintf("/data/links/%c/%s",x[0],x)
#define USER_PATH(x)		sprintf("/data/players/%c/%s",x[0],x)
#define PRELOAD_FILE		"/data/config/preload"

#define WIZ_DIR			"/wiz"
#define LOG_DIR			"/log"


#define BANISH_LOG		LOG_DIR "/banishes"
#define WELCOME_FILE		"/data/config/WELCOME"
#define NEW_PLAYER		"/data/config/NEW_PLAYER"
#define MOTD_FILE		"/data/config/MOTD"

#endif /* __CONFIG_H__ */
