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

#define START			"/domains/std/rooms/beach/Sandy_Beach"
#define WIZARD_START		"/domains/std/Wizroom"

#define LOGIN_PROMPT		"Please enter your username: "

/* ! will give you problems since it's used as input escape. */
#define HISTORY_CHAR		'%'

#define EVERYTHING_SAVES	/* Note: if you define this, you have to
				   modify the lib to save the correct vars */

#define OBVIOUS_EXITS		/* show obvious exits when looking (top) */
#undef OBVIOUS_EXITS_BOTTOM	/* show obvious exits at bottom of long() */

#define DEFAULT_LIGHT_LEVEL	1	/* a room's default light */

#define EVERYONE_HAS_A_PLAN
#undef PLAYER_INFO_IS_PUBLIC	/* are names email addrs avail to I3 and
				   players via finger? */

#define PLAYERS_START_WHERE_THEY_QUIT

#define USE_WIZ_POSITION	/* use the "position" scheme for wizzes */
#define USE_QUESTS		/* quests are not always related to gaming,
				   so we have a separate config flag */
#define USE_STATUS_LINE            /* Allow use of Infocom-like Status Line */
#define USE_GAME_FEATURES	/* use a lot of features related to gaming */

#ifdef USE_GAME_FEATURES	/* pull in a set for gaming features */
# define USE_RACES		// Implement races for PC's only
# define USE_STATS
# define USE_GUILDS
# define USE_SKILLS
# define USE_TITLES
# define USE_BODYSLOTS
# undef USE_SIMPLE_LEVEL	// use the simple level number scheme for players
# undef USE_MON_LEVEL		// use the simeple level number scheme for monsters
# undef USE_TEAMS		// use team support
# undef USE_SIMPLE_EXP		// use the simple experience scheme for players
# undef USE_MON_RACES		// use monster race support
#endif

/* one or the other should be defined */
#define USE_SIZE

#ifndef USE_SIZE  // Mass and Size do the same thing, so you can only use 1
# define USE_MASS
#endif

/*
** This is the "maximum" skill level that players are compared against.
** Their evaluation percentage will be computed as a total of all their
** skill values relative to this value.  This value should be somewhere
** between 0 and /std/body/skills.c::MAX_SKILL_VALUE * <number of skills>.
** Typically, it wouldn't change but a mud that is just starting out might
** adjust this (causing a reevaluation of everybody) if a number of players
** are reaching or exceeding this value (which is quite legal).  This value
** is simply an aid for players to measure progress against (setting it to
** the maximum would mean that players wouldn't really ever budge anywhere).
*/
#define EVALUATION_SKILL_LEVEL	50000

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
#define FEEDBACK_NEWSGROUP "reporter.discussion"


/* Define this if you want news data to be stored in the user object rather 
 * than the body.  If you are using multiple bodies for a user object, this 
 * it is recommended that you undef this.  If you do define this, and previously used 
 * to store newsdata in the user object you need to make sure that both /std/body.c and
 * /secure/user.c inherit /secure/user/newsdata.c */
#undef NEWS_DATA_IN_USER

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


// The following are to set directory names and short mudname used by
// the autodoc daemon
// You will need to make a dir of MUD_AUTODOC_DIR in /help/autodoc
// If you plan to make use of this feature
#define AUTODOC_MUDNAME "LIMA"
#define MUD_AUTODOC_DIR "lima"
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
