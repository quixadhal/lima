/* Do not remove the headers from this file! see /USAGE for more info. */

#include <config.h>

#define NONE		0
#define TYRO		1
#define NOVICE		2
#define INTERMEDIATE	3
#define EXPERT		4
#define MASTER		5

#ifdef AUTO_WIZ
#define LEVELS ({ "guestwiz", "apprentice", "wizard", "adept", "archwiz", "admin"})
#else
#define LEVELS 	({ "player", "guestwiz", "apprentice", "wizard",\
	"archwiz", "admin"})
#endif
