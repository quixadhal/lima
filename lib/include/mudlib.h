/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __MUDLIB_H__
#define __MUDLIB_H__
#pragma no_warnings

#include <daemons.h>
#include <dirs.h>
#include <size.h>
#include <setbit.h>
#include <msgtypes.h>
#include <classes.h>
#include <lpscript.h>

/*
** BODY		main player object (body)
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
** FURNITURE
** MOUNT	
** WEAPON	weapons
** ARMOR	armor
** CONTAINER	obs that can hold things in/on/under/behind/etc.
** DOOR		doors to places
** SECRET_DOOR        secret doors.
** LIVING	can hold things and perform actions (no fighting)
** PAPER      paper .. WFW
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
** M_ANSI       function for using the status line (Infocom style header)
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
** M_AGGRESSIVE
** M_KNOCKABLE
** M_VALUABLE	items that are valuable (have value)
** M_MOUNTABLE
** M_ENTERABLE
** M_FLOW_ROOM
** M_GUILD_MASTER	act as a "guild master"
** M_SIBLING          objects that exist in two rooms;
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
** SIMPLE_OB	an object with simple responses to verbs
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
** ACCOUNTANT
** SWORD
** BOOK
** LADDER
** STAIRS
** TORCH
** VEHICLE
** HOLE
*/

#define BODY			"/std/body"
#define USER_OB			"/secure/user"
#define USER_OB_BAK		"/secure/user_bak"

#define SIMUL_OB		"/secure/simul_efun"

#define BASE_ROOM		"/std/base_room"
#define INDOOR_ROOM		"/std/indoor_room"
#define OUTDOOR_ROOM		"/std/outdoor_room"
#define NON_ROOM		"/std/non_room"
#define WATER_ROOM		"/std/water_room"
#define GRID_SERVER		"/std/grid_server"

//### temporary
#define ROOM			INDOOR_ROOM

#define VOID_ROOM		"/domains/std/Void"
#define WIZ_ROOM		"/domains/std/Wizroom"

#define ADVERSARY               "/std/adversary"
#define AGGRESSIVE_MONSTER      "/std/aggressive_monster"
#define ARMOR			"/std/armor"
#define BASE_OBJ		"/std/base_obj"
#define CHEST			"/std/chest"
#define COMBAT_SPELL            "/std/combat_spell"
#define CONTAINER		"/std/container"
#define CORPSE			"/std/corpse"
#define EXIT_OBJ                 "/std/complex_exit_obj"
#define COMPLEX_EXIT_OBJ        "/std/complex_exit_obj"
#define DOOR			"/std/door"
#define SECRET_DOOR             "/std/secret_door"
#define FLAME			"/std/flame"
#define HIDDEN_DOOR             "/std/hidden_door"
#define HIDDEN_EXIT             "/std/hidden_exit"
#define FLUID                   "/std/fluid"
#define FOOD                    "/std/food"
#define FURNITURE		"/std/furniture"
#define GUILD_GUARD             "/std/guild_guard"
#define KEY                     "/std/key"
#define LIGHT_OBJECT            "/std/light_object"
#define LIVING			"/std/living"
#define	MENUS			"/std/menu"
#define MOVING_ROOM             "/std/moving_room"
#define MOVING_MONSTER          "/std/moving_monster"
#define FOLLOWING_MONSTER       "/std/following_monster"
#define WANDERING_MONSTER       "/std/wandering_monster"
#define MOUNT			"/std/mount"
#define OBJ 			"/std/object"
#define PAPER                  "/std/paper"
#define PARAM_OBJ		"/std/param_obj"
#define RACE			"/std/race"
#define SIGN                    "/std/sign"
#define SPELL			"/std/spell"
#define STOCK_MASTER            "/std/stock_master"
#define VERB_OB			"/std/verb_ob"
#define NVERB_OB		"/std/nverb_ob"
#define WEAPON			"/std/weapon"

/* used by the OBJ object */

/* for mudlib objects */
#define M_ANSI          	"/std/modules/m_ansi"
#define M_BODY_STATS		"/std/modules/m_bodystats"
#define M_COMPLETE		"/std/modules/m_complete"
#define M_GLOB		        "/std/modules/m_glob"
#define M_GRAMMAR		"/std/modules/m_grammar"
#define M_INPUT			"/secure/modules/m_input"
#define M_ITEMS			"/std/modules/m_items"
#define M_PARSING		"/std/modules/m_parsing"
#define M_RECONNECT		"/std/modules/m_reconnect"
#define M_REGEX			"/std/modules/m_regex"
#define M_SAVE			"/secure/modules/m_save"
#define M_SET			"/secure/modules/m_set"
#define M_SMARTMOVE		"/std/modules/m_smartmove"
#define M_EDIBLE		"/std/modules/m_edible"
#define M_DRINKABLE		"/std/modules/m_drinkable"
#define M_DRINK_CONTAINER	"/std/modules/m_drink_container"
#define M_DRINK_SOURCE          "/std/modules/m_drink_source"
#define M_FLUID_SOURCE          "/std/modules/m_fluid_source"
#define M_FLUID_CONTAINER       "/std/modules/m_fluid_container"
#define M_CONVERSATION		"/std/modules/m_conversation"

/* for area objects */
#define M_ACCOUNTANT            "/std/modules/m_accountant"
#define M_ACTIONS               "/std/modules/m_actions"
#define M_BLOCKEXITS		"/std/modules/m_blockexits"
#define M_AGGRESSIVE		"/std/modules/m_aggressive"
#define M_DAMAGE_SINK		"/std/modules/m_damage_sink"
#define M_DAMAGE_SOURCE		"/std/modules/m_damage_source"
#define M_DIGGABLE		"/std/modules/m_diggable"
#define M_DECAY                 "/std/modules/m_decay"
#define M_DIGGER		"/std/modules/m_digger"
#define M_FLOW_ROOM		"/std/modules/m_flow_room"
#define M_FOLLOW                "/std/modules/m_follow"
#define M_GETTABLE		"/std/modules/m_gettable"
#define M_GUILD_MASTER          "/std/modules/m_guild_master"
#define M_KNOCKABLE		"/std/modules/m_knockable"
#define M_LIGHT_SOURCE          "/std/modules/m_light_source"
#define M_LIGHTABLE		"/std/modules/m_lightable"
#define M_LOCKABLE		"/std/modules/m_lockable"
#define M_SEARCHABLE		"/std/modules/m_searchable"
#define M_MESSAGES		"/std/modules/m_messages"
#define M_MOUNTABLE		"/std/modules/m_mountable"
#define M_OPENABLE		"/std/modules/m_openable"
#define M_READABLE		"/std/modules/m_readable"
#define M_SIBLING               "/std/modules/m_sibling"
#define M_SWITCHABLE		"/std/modules/m_switchable"
#define M_THROWABLE		"/std/modules/m_throwable"
#define M_TRIGGERS		"/std/modules/m_triggers"
#define M_VALUABLE		"/std/modules/m_valuable"
#define M_VENDOR		"/std/modules/m_vendor"
#define M_WANDER                "/std/modules/m_wander"
#define M_WANDER_OLD            "/std/modules/m_wander_old"
#define M_WEARABLE		"/std/modules/m_wearable"
#define M_WIELDABLE		"/std/modules/m_wieldable"
#define M_WRITING_SURFACE	"/std/modules/m_writing_surface"
#define M_EXIT                  "/std/modules/m_exit"
#define M_COMPLEX_EXIT          "/std/modules/m_complex_exit"

#define M_LIB_LOCKABLE		"/domains/std/lockable"
#define M_LIB_OPENABLE		"/domains/std/openable"

#define ED_SESSION		"/trans/obj/ed_session"
#define ADDTO_OB		"/trans/obj/addto_ob"
#define WIZ_SHELL		"/trans/obj/wish"
#define TELNET_OB		"/trans/obj/telnet_ob"
#define ADMTOOL			"/obj/admtool/admtool2"
#define TASKTOOL                "/obj/tasktool/tasktool"
#define DATAEDIT		"/trans/obj/dataedit"

#define ALIASMENU		"/obj/mudlib/aliasmenu"
#define ANNO_MENU		"/obj/mudlib/annotationmenu"
#define HELPSYS			"/obj/mudlib/helpsys"
#define HINT_MENU		"/obj/mudlib/hintmenu"
#define IFTP_OB			"/obj/mudlib/iftp_ob"
#define NEWSREADER		"/obj/mudlib/newsreader"
#define NNTP_OB			"/obj/mudlib/nntp"
#define PLAYER_MENU		"/obj/mudlib/plmenu"
#define PLYR_SHELL		"/obj/mudlib/pshell"
#define SIMPLE_OB		"/obj/mudlib/simple_ob"
#define TEAM_MENU               "/obj/mudlib/team_menu"
#define TEMP_WORKROOM		"/obj/mudlib/TWorkroom"
#define WATER			"/std/water"

#define M_ACCESS		"/secure/modules/m_access"
#define M_FILE			"/secure/modules/m_file"
#define M_DAEMON_DATA           "/secure/modules/m_daemon_data"

#define CMD			"/obj/secure/cmd"
#define MAILBOX			"/obj/secure/mailbox"
#define MAILER_DIR		"/obj/secure/mailers"
#define MAILER			MAILER_DIR "/mailer"
#define SOCKET			"/obj/secure/socket"

#define SHELL		 	"/obj/secure/shell"
#define M_ALIAS			"/obj/secure/shell/alias"
#define M_HISTORY	   	"/obj/secure/shell/history"
#define M_SCROLLBACK		"/obj/secure/shell/scrollback"
#define M_SHELLFUNCS		"/obj/secure/shell/shellfuncs"

/* used by 'wish';  M_GETOPT also used in CMD_D */
#define M_GETOPT		"/obj/secure/shell/getopt"
#define M_PROMPT		"/obj/secure/shell/prompt"
#define M_SHELLVARS 		"/obj/secure/shell/shellvars"

// These next few are 'high level' inheritables and probably should have
// their own dir.
#define ACCOUNTANT              "/std/accountant"
#define BED			"/std/bed"
#define MONEY                   "/std/money"
#define BOOK			"/std/book"
#define COINS			"/std/coins"
#define HOLE			"/std/hole"
#define LADDER			"/std/ladder"
#define LANTERN                 "/std/lantern"
#define PORTAL			"/std/portal"
#define STAIRS			"/std/stairs"
#define SWORD			"/std/sword"
#define TORCH			"/std/torch"
#define VEHICLE			"/std/vehicle"
#define INF_SUPPLY              "/std/inf_supply"

#endif /* __MUDLIB_H__ */
