/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __MUDLIB_H__
#define __MUDLIB_H__

#include <daemons.h>
#include <dirs.h>
#include <size.h>
#include <setbit.h>
#include <msgtypes.h>
#include <classes.h>

/*
** BODY		main player object (body)
** GHOST	ghost object (body)
** USER_OB	user's connection object
** USER_OB_BAK	user's connection object (backup version)
**
** SIMUL_OB	the simul efun object
**
** BASE_ROOM	the room rooms are built from...
** INDOOR_ROOM	standard room (indoors)
** OUTDOOR_ROOM	an outdoor room/environment
** NON_ROOM	base class for things like furniture
** WATER_ROOM	rooms full of water (e.g. a lake)
** GRID_SERVER	virtual server inheritable for grid-type rooms
**
** VOID_ROOM	where to put broken stuff
** WIZ_ROOM	a room for wizards
**
** BASE_OBJ	base class for objects
** OBJ		generic objects
** SPARSE_OBJ	simple do-nothing objects.
** LESS_SPARSE_OBJ
** FURNITURE
** MOUNT	
** WEAPON	weapons
** ARMOR	armor
** MONSTER	monsters -- living objects that fight
** CONTAINER	obs that can hold things in/on/under/behind/etc.
** DOOR		doors to places
** LIVING	can hold things and perform actions (no fighting)
** STOCK_MASTER  stock guildmaster for stock muds :)
** RACE		a race-modified body
** COINS	a bunch of coins
** SPELL	base class for spells
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
** M_SMARTMOVE	living's movement with directions and enter/exit msgs
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
** M_DIGGER	items that can dig.
** M_DIGGABLE   items that you can dig in.
** M_BLOCKEXITS
** M_BLOCKABLE
** M_KNOCKABLE
** M_VALUABLE	items that are valuable (have value)
** M_MOUNTABLE
** M_ENTERABLE
** M_GUILD_MASTER	act as a "guild master"
**
** M_LIB_LOCKABLE	arbitrates "locked" state of multiple instances
** M_LIB_OPENABLE	arbitrates "opened" state of multiple instances
**
** ED_SESSION	provides an 'ed' session for the user
** ADDTO_OB	object to add ines to a file
** WIZ_SHELL	a wizard's shell object
** TELNET_OB	a telnet tool
** ADMTOOL	administration tool's input menu object
** DATAEDIT	save file data editor
**
** ALIASMENU
** ANNO_MENU
** CORPSE
** HELPSYS	help system
** HINT_MENU
** IFTP_OB	command object to transfer files between muds via I3
** NEWSREADER	a basic newsreader
** NNTP_OB
** PLAYER_MENU
** PLYR_SHELL
** TEMP_WORKROOM
** WATER	some water
**
** VERB_OB	basic functionality for parser-based verbs
** MENUS   	generic menuing facility
**
** CMD          base inheritable for commands
** DAEMON	for daemons
** MAILBOX	mailbox to hold message references
** MAILER	generic "mailer" functionality
** SOCKET	object to manage a socket
**
** SHELL
** M_ALIAS 	module for storing and expanding aliases
** M_GETOPT	functions for command processing
** M_HISTORY 	module for handling command history
** M_PROMPT
** M_SCROLLBACK
** M_SHELLFUNCS	??
** M_SHELLVARS	??
**
** SWORD
** BOOK
** LADDER
** STAIRS
** TORCH
** VEHICLE
*/

#define BODY			"/std/body"
#define GHOST			"/std/ghost"	// ### bogus
#define USER_OB			"/secure/user"
#define USER_OB_BAK		"/secure/user_bak"

#define SIMUL_OB		"/secure/simul_efun"

#define BASE_ROOM		"/std/base_room"
#define INDOOR_ROOM		"/std/indoor_room"
#define OUTDOOR_ROOM		"/std/outdoor_room"
#define NON_ROOM		"/std/non-room"
#define WATER_ROOM		"/std/water_room"
#define GRID_SERVER		"/std/grid_server"

//### temporary
#define ROOM			"/std/room/room"

#define VOID_ROOM		"/domains/std/void"
#define WIZ_ROOM		"/domains/std/wizroom"

#define ARMOR			"/std/armor"
#define BASE_OBJ		"/std/base_obj"
#define CONTAINER		"/std/container"
#define COMPLEX_CONTAINER	"/std/complex_container"
#define DOOR			"/std/door"
#define FURNITURE		"/std/furniture"
#define LESS_SPARSE_OBJ		"/std/less_sparse_obj"
#define LIVING			"/std/living"
#define	MENUS			"/std/menu"
#define MONSTER			"/std/monster"
#define MOUNT			"/std/mount"
#define OBJ 			"/std/object"
#define RACE			"/std/race"
#define SPARSE_OBJ		"/std/sparse_obj"
#define SPELL			"/std/spell"
#define STOCK_MASTER            "/std/stock_master"
#define VERB_OB			"/std/verb_ob"
#define WEAPON			"/std/weapon"

/* for mudlib objects */
#define M_ANSI          	"/std/modules/m_ansi"
#define M_BODY_STATS		"/std/modules/m_bodystats"
#define M_COMPLETE		"/std/modules/m_complete"
#define M_GLOB		        "/std/modules/m_glob"
#define M_GRAMMAR		"/std/modules/m_grammar"
#define M_INPUT			"/std/modules/m_input"
#define M_ITEMS			"/std/modules/m_items"
#define M_PARSING		"/std/modules/m_parsing"
#define M_RECONNECT		"/std/modules/m_reconnect"
#define M_REGEX			"/std/modules/m_regex"
#define M_SAVE			"/std/modules/m_save"
#define M_SET			"/std/modules/m_set"
#define M_SMARTMOVE		"/std/modules/m_smartmove"

/* for area objects */
#define M_ACTIONS               "/std/modules/m_actions"
#define M_BLOCKEXITS		"/std/modules/m_blockexits"
#define M_CLIMBABLE		"/std/modules/m_climbable"
#define M_DAMAGE_SINK		"/std/modules/m_damage_sink"
#define M_DAMAGE_SOURCE		"/std/modules/m_damage_source"
#define M_DIGGABLE		"/std/modules/m_diggable"
#define M_DIGGER		"/std/modules/m_digger"
#define M_ENTERABLE		"/std/modules/m_enterable"
#define M_GETTABLE		"/std/modules/m_gettable"
#define M_GUILD_MASTER          "/std/modules/m_guild_master"
#define M_KNOCKABLE		"/std/modules/m_knockable"
#define M_LIGHTABLE		"/std/modules/m_lightable"
#define M_LOCKABLE		"/std/modules/m_lockable"
#define M_MESSAGES		"/std/modules/m_messages"
#define M_MOUNTABLE		"/std/modules/m_mountable"
#define M_OPENABLE		"/std/modules/m_openable"
#define M_READABLE		"/std/modules/m_readable"
#define M_SWITCHABLE		"/std/modules/m_switchable"
#define M_THROWABLE		"/std/modules/m_throwable"
#define M_TRIGGERS		"/std/modules/m_triggers"
#define M_VALUABLE		"/std/modules/m_valuable"
#define M_VENDOR		"/std/modules/m_vendor"
#define M_WANDER                "/std/modules/m_wander"
#define M_WEARABLE		"/std/modules/m_wearable"
#define M_WIELDABLE		"/std/modules/m_wieldable"
#define M_WRITING_SURFACE	"/std/modules/m_writing_surface"

#define M_LIB_LOCKABLE		"/domains/std/lockable"
#define M_LIB_OPENABLE		"/domains/std/openable"

#define ED_SESSION		"/trans/obj/ed_session"
#define ADDTO_OB		"/trans/obj/addto_ob"
#define WIZ_SHELL		"/trans/obj/wish"
#define TELNET_OB		"/trans/obj/telnet_ob"
#define ADMTOOL			"/trans/obj/admtool"
#define DATAEDIT		"/trans/obj/dataedit"

#define ALIASMENU		"/obj/aliasmenu"
#define ANNO_MENU		"/obj/annotationmenu"
#define CORPSE			"/obj/corpse"
#define HELPSYS			"/obj/helpsys"
#define HINT_MENU		"/obj/hintmenu"
#define IFTP_OB			"/obj/iftp_ob"
#define NEWSREADER		"/obj/newsreader"
#define NNTP_OB			"/obj/nntp"
#define PLAYER_MENU		"/obj/plmenu"
#define PLYR_SHELL		"/obj/pshell"
#define TEMP_WORKROOM		"/obj/tworkroom"
#define WATER			"/obj/water"

#define M_ACCESS		"/secure/modules/access"

#define CMD			"/secure/obj/cmd"
#define DAEMON			"/secure/obj/daemon"
#define MAILBOX			"/secure/obj/mailbox"
#define MAILER			"/secure/obj/mailers/mailer"
#define SOCKET			"/secure/obj/socket"

#define SHELL		 	"/secure/obj/shell"
#define M_ALIAS			"/secure/obj/shell/alias"
#define M_HISTORY	   	"/secure/obj/shell/history"
#define M_SCROLLBACK		"/secure/obj/shell/scrollback"
#define M_SHELLFUNCS		"/secure/obj/shell/shellfuncs"

/* used by 'wish';  M_GETOPT also used in CMD_D */
#define M_GETOPT		"/secure/obj/shell/getopt"
#define M_PROMPT		"/secure/obj/shell/prompt"
#define M_SHELLVARS 		"/secure/obj/shell/shellvars"

// These next few are 'high level' inheritables and probably should have
// their own dir.
#define BOOK			"/std/book"
#define COINS			"/std/coins"
#define LADDER			"/std/ladder"
#define PORTAL			"/std/portal"
#define STAIRS			"/std/stairs"
#define SWORD			"/std/sword"
#define TORCH			"/std/torch"
#define VEHICLE			"/std/vehicle"

#endif /* __MUDLIB_H__ */
