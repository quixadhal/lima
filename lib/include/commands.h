/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** commands.h -- Command related paths and objects
**
** Only the objects directly referenced by portions of the lib are
** defined explicitly.
**
** 950709, Deathblade: Created.
** 950715, Rust: changed to /cmds
*/

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

/*
** Command paths
*/
#define CMD_DIR			"/cmds"
#define TRANS_CMD_DIR		"/trans/cmds"
#define TRANS_ADMINCMD_DIR	"/trans/admincmds"

#define CMD_DIR_PLAYER		CMD_DIR "/player"
#define CMD_DIR_VERBS		CMD_DIR "/verbs"
#define CMD_DIR_RESTRICT	CMD_DIR "/wiz"
#define CMD_DIR_NO_RESTRICT	TRANS_CMD_DIR
#define CMD_DIR_CREATE          CMD_DIR "/create"
#define CMD_DIR_ADMIN           TRANS_ADMINCMD_DIR

/*
** Command objects
*/
#define CMD_OB_ADVERBS		CMD_DIR_PLAYER "/adverbs"
#define CMD_OB_ANSI		CMD_DIR_PLAYER "/ansi"
#define CMD_OB_BIFF		CMD_DIR_PLAYER "/biff"
#define CMD_OB_CHAN		CMD_DIR_PLAYER "/chan"
#define CMD_OB_DESCRIBE		CMD_DIR_PLAYER "/describe"
#define CMD_OB_EMOTEAPROPOS	CMD_DIR_PLAYER "/emoteapropos"
#define CMD_OB_FEELINGS		CMD_DIR_PLAYER "/feelings"
#define CMD_OB_FINGER		CMD_DIR_PLAYER "/finger"
#define CMD_OB_NICKNAME		CMD_DIR_PLAYER "/nickname"
#define CMD_OB_PASSWD		CMD_DIR_PLAYER "/passwd"
#define CMD_OB_SEMOTE		CMD_DIR_PLAYER "/semote"
#define CMD_OB_SNOOPABLE	CMD_DIR_PLAYER "/snoopable"
#define CMD_OB_SU		CMD_DIR_PLAYER "/su"
#define CMD_OB_TELL		CMD_DIR_PLAYER "/tell"

#define CMD_OB_SNOOP		CMD_DIR_RESTRICT "/snoop"

#define CMD_OB_ADMTOOL		CMD_DIR_RESTRICT "/admtool"
#define CMD_OB_FINGER_REAL	CMD_DIR_NO_RESTRICT "/finger"
#define CMD_OB_LS		CMD_DIR_NO_RESTRICT "/ls"
#define CMD_OB_TASKTOOL		CMD_DIR_RESTRICT "/tasktool"
#define CMD_OB_UPDATE		CMD_DIR_NO_RESTRICT "/update"

/*
** Verb objects
*/
#define VERB_OB_CAST            CMD_DIR_VERBS "/cast"
#define VERB_OB_GET		CMD_DIR_VERBS "/get"
#define VERB_OB_GO		CMD_DIR_VERBS "/go"
#define VERB_OB_LOOK		CMD_DIR_VERBS "/look"

#endif /* __COMMANDS_H__ */
