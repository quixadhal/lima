/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __MUDLIB_H__
#define __MUDLIB_H__

#include <daemons.h>
#include <dirs.h>
#include <size.h>
#include <setbit.h>

/*
** PLAYER	main player object (body)
** GHOST	ghost object (body)
** USER_OB	user's connection object
** USER_OB_BAK	user's connection object (backup version)
**
** SIMUL_OB	the simul efun object
**
** ROOM		standard room
** ENTERABLE	enterable objects that behave like a room (e.g. a boat)
** WATER_ROOM	rooms full of water (e.g. a lake)
**
** VOID_ROOM	where to put broken stuff
** WIZ_ROOM	a room for wizards
**
** OBJ		generic objects
** SPARSE_OBJ	simple do-nothing objects.
** WEAPON	weapons
** ARMOR	armor
** MONSTER	monsters -- living objects that fight
** CONTAINER	obs that can hold things in/on/under/behind/etc.
** DOOR		doors to places
** LIVING	can hold things and perform actions (no fighting)
** RACE		a race-modified body
**
** M_GLOB	glob style regexp functions
** M_GRAMMAR	grammatical manipulation functions
** M_CMDIO	i/o functions for cmds
** M_PARSING	input parsing functions
** M_REGEX	regular expression functions
** M_INPUT	interface to the input system
** M_COMPLETE	name completion
** M_RECONNECT	functions for performing socket reconnects
** M_ANSI	functions for using the %^ ansi protocol
** M_SAVE	functions to abstract saveing and restoring objects
**
** M_BODY_STATS	statistics (characteristics) for a body (player/NPC)
**
** M_WEARABLE	things that can be worn
** M_SWITCHABLE
** M_LIGHTABLE
** M_ITEMS	provides virtual/fake items for an object
** M_GETTABLE	objects that can be picked up
** M_THROWABLE  objects that can be thrown
** M_OPENABLE	objects that can be opened/closed
** M_LOCKABLE	openables that can be locked
** M_READABLE	something that can be read
** M_DAMAGE_SOURCE
** M_DAMAGE_SINK
** M_WIELDABLE	something that can be wielded (a weapon)
** M_MESSAGES	message generation
** M_ACTIONS
** M_TRIGGERS
** M_WRITING_SURFACE
** M_VENDOR
** M_OUTSIDE	outside of an object that also has an inside
** M_DIGGER	items that can dig.
** M_DIGGABLE   items that you can dig in.
**
** M_GETOPT	functions for command processing
** M_HISTORY 	module for handling command history
** M_ALIAS 	module for storing and expanding aliases
** M_SCROLLBACK
** M_PROMPT
** M_SHELLVARS	??
** M_ENVVARS	??
** M_CHAR_BINDINGS	??
** M_SHELLFUNCS	??
**
** M_LIB_LOCKABLE	arbitrates "locked" state of multiple instances
** M_LIB_OPENABLE	arbitrates "opened" state of multiple instances
**
** EDIT_OB	provides text-entry functionality
** ED_SESSION	provides an 'ed' session for the user
** ADDTO_OB	object to add ines to a file
** WIZ_SHELL	a wizard's shell object
** TELNET_OB	a telnet tool
**
** HELPSYS	help system
** NEWSREADER	a basic newsreader
** FAKE_OB
** TEMP_WORKROOM
** ALIASMENU
**
** REPORTER	for reporting (logging) bugs/ideas/typos
** VERB_OB	basic functionality for parser-based verbs
** DAEMON	for daemons
** SOCKET	object to manage a socket
** MAILER	generic "mailer" functionality
** SHELL
** MENUS   	generic menuing facility
** CMD          base inheritable for commands
**
** MAILBOX	mailbox to hold message references
**
** PLAYER_MENU
** PLYR_SHELL
**
** SWORD
** BOOK
*/

#define PLAYER			"/std/player"
#define GHOST			"/std/ghost"
#define USER_OB			"/secure/user"
#define USER_OB_BAK		"/secure/user_bak"

#define SIMUL_OB		"/secure/simul_efun"

#define ROOM			"/std/room"
#define INSIDE			"/std/inside"
#define WATER_ROOM		"/std/water_room"

#define VOID_ROOM		"/domains/std/void"
#define WIZ_ROOM		"/domains/std/wizroom"

#define OBJ 			"/std/object"
#define SPARSE_OBJ		"/std/sparse_obj"
#define WEAPON			"/std/weapon"
#define ARMOR			"/std/armor"
#define MONSTER			"/std/monster"
#define CONTAINER		"/std/container"
#define DOOR			"/std/door"
#define LIVING			"/std/living"
#define RACE			"/std/race"

#define M_GLOB		        "/std/modules/glob"
#define M_GRAMMAR		"/std/modules/grammar"
#define M_CMDIO			"/std/modules/cmdio"
#define M_PARSING		"/std/modules/parsing"
#define M_REGEX			"/std/modules/regex"
#define M_INPUT			"/std/modules/input"
#define M_COMPLETE		"/std/modules/complete"
#define M_RECONNECT		"/std/modules/reconnect"
#define M_ANSI          	"/std/modules/ansi"
#define M_SAVE			"/std/modules/save"

#define M_BODY_STATS		"/std/modules/bodystats"
#define M_SMARTMOVE		"/std/modules/smartmove"

#define M_WEARABLE		"/std/modules/wearable"
#define M_SWITCHABLE		"/std/modules/switchable"
#define M_LIGHTABLE		"/std/modules/lightable"
#define M_ITEMS			"/std/modules/items"
#define M_GETTABLE		"/std/modules/gettable"
#define M_THROWABLE		"/std/modules/throwable"
#define M_OPENABLE		"/std/modules/openable"
#define M_CLIMBABLE		"/std/modules/climbable"
#define M_LOCKABLE		"/std/modules/lockable"
#define M_READABLE		"/std/modules/readable"
#define M_DAMAGE_SOURCE		"/std/modules/damage_source"
#define M_DAMAGE_SINK		"/std/modules/damage_sink"
#define M_WIELDABLE		"/std/modules/wieldable"
#define M_MESSAGES		"/std/modules/messages"
#define M_ACTIONS               "/std/modules/actions"
#define M_TRIGGERS		"/std/modules/triggers"
#define M_WRITING_SURFACE	"/std/modules/writing_surface"
#define M_VENDOR		"/std/modules/vendor"
#define M_OUTSIDE		"/std/modules/outside"
#define M_DIGGER		"/std/modules/digger"
#define M_DIGGABLE		"/std/modules/diggable"

#define M_GETOPT		"/std/shell/getopt"
#define M_HISTORY	   	"/std/shell/history"
#define M_ALIAS			"/std/shell/alias"
#define M_SCROLLBACK		"/std/shell/scrollback"
#define M_PROMPT		"/std/shell/prompt"
#define M_SHELLVARS 		"/std/shell/shellvars"
#define M_ENVVARS		"/std/shell/envvars"
#define M_CHAR_BINDINGS 	"/std/shell/bindings"
#define M_SHELLFUNCS		"/std/shell/shellfuncs"

#define M_LIB_LOCKABLE		"/domains/std/lockable"
#define M_LIB_OPENABLE		"/domains/std/openable"

#define EDIT_OB			"/trans/obj/edit_ob"
#define ED_SESSION		"/trans/obj/ed_session"
#define ADDTO_OB		"/trans/obj/addto_ob"
#define WIZ_SHELL		"/trans/obj/wish"
#define TELNET_OB		"/trans/obj/telnet_ob"

#define HELPSYS			"/obj/helpsys"
#define NEWSREADER		"/obj/newsreader"
#define FAKE_OB			"/obj/fake"
#define TEMP_WORKROOM		"/obj/tworkroom"
#define ALIASMENU		"/obj/aliasmenu"

#define REPORTER		"/std/reporter"
#define VERB_OB			"/std/verb_ob"
#define DAEMON			"/std/daemon"
#define SOCKET			"/std/socket"
#define MAILER			"/std/mailers/mailer"
#define SHELL		 	"/std/shell"
#define	MENUS			"/std/menu"
#define CMD			"/std/cmd"

#define MAILBOX			"/secure/mailbox"

#define PLAYER_MENU		"/obj/plmenu"
#define PLYR_SHELL		"/obj/pshell"

// These next few are 'high level' inheritables and probably should have
// their own dir.
#define SWORD			"/std/sword"
#define BOOK			"/std/book"
#define LADDER			"/std/ladder"
#define STAIRS			"/std/stairs"
#define TORCH			"/std/torch"
#define VEHICLE			"/std/vehicle"

#endif /* __MUDLIB_H__ */
