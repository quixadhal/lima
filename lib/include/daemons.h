/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __DAEMONS_H__
#define __DAEMONS_H__

#define ALIAS_D		"/daemons/alias"
#define ANNOTATION_D    "/daemons/annotation"
#define ANNOUNCE_D	"/daemons/announce"
#define BANISH_D	"/daemons/banish"
#define CHANLOG_D	"/daemons/chanlog"
#define CHANNEL_D	"/daemons/nchannel"
#define DID_D           "/daemons/did"
#define DOC_D		"/daemons/doc"
#define GROUP_D		"/daemons/group"
#define GUILD_D		"/daemons/guild"
#define HELP_D		"/daemons/help"
#define IMUD_D		"/daemons/imud"
#define LAST_LOGIN_D	"/daemons/last_login"
#define MAILBOX_D	"/daemons/mailbox"
#define MESSAGES_D	"/daemons/messages"
#define MONEY_D		"/daemons/money"
#define NEWS_D		"/daemons/news"
#define QUEST_D		"/daemons/quest"
#define REPORTER_D	"/daemons/reporter"
#define SOUL_D		"/daemons/soul"
#define SPELL_D		"/daemons/spell"
#define TIMER_D		"/daemons/timer"
#define WEATHER_D	"/daemons/weather"

#define CMD_D		"/secure/daemons/cmd"
#define FINGER_D	"/secure/daemons/finger"
#define LOG_D		"/secure/daemons/log"
#define MAIL_D		"/secure/daemons/mail"
#define NEW_USER_D	"/secure/daemons/new_user"
#define USER_D		"/secure/daemons/user"

// Define these only if you want intermud services.
#define REMOTE_D        "/secure/daemons/remote"
#define CHANGW_D	"/daemons/changw"

// ### obsolete
#define NCHANNEL_D CHANNEL_D

#endif // __DAEMONS_H__
