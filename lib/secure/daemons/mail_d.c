/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Belboz on July 16 for Zorkmud, something that any mailer can
** interface with, so Beek and I can have our input_to()less mailer.
**
** 950710, Deathblade: Added mailboxes, removed mail queue.
** 950910, Deathblade: revamped some more: doc, trim api, cleaning
** 960119, Deathblade: moved mailbox functionality to MAILBOX_D
**
** Public API:
**
**   send_mail(): send a piece of mail
**   delete_mail(): delete a piece of mail for a user
**   get_one_message(): get a message
**
**   rebuild_mailboxes(): rebuild all mailboxes from actual message data
*/

#include <mudlib.h>
#include <classes.h>

inherit DAEMON;
inherit CLASS_MAILMSG;

#define MAIL_PATH	"/data/M/"

private class mail_msg saved_msg;

private static mapping mailboxes = ([ ]);


private nomask void create()
{
    object mailbox;

    foreach ( mailbox in children(MAILBOX) )
    {
	string owner;

	if ( mailbox && (owner = mailbox->query_owner()) )
	    mailboxes[owner] = mailbox;
    }
}

private nomask string get_fname(int message_key)
{
//### eventually move this to /data/mail/N/N/NNNNNNNN.o
    return sprintf(MAIL_PATH "%d", message_key);
}

private nomask void save_msg(int message_key, class mail_msg msg)
{
    saved_msg = msg;
    unguarded(1, (: save_object, get_fname(message_key) :));
    saved_msg = 0;
}

/* old style saved message */
private string * to_list;
private string * cc_list;
private string   sender;
private string   subject;
private string * body;
private string * dels_pending;

private nomask class mail_msg restore_msg(int message_key)
{
    class mail_msg msg;

    if ( !unguarded(1, (: restore_object, get_fname(message_key), 1 :)) )
	return 0;

    msg = saved_msg;
    if ( !msg )
    {
	msg = new(class mail_msg);
	msg->to_list		= to_list;
	msg->cc_list		= cc_list;
	msg->sender		= sender;
	msg->date		= message_key;
	msg->subject		= subject;
	msg->thread_id		= message_key;
	msg->body		= body;
	msg->dels_pending	= dels_pending;

	to_list = cc_list = sender = subject = body = dels_pending = 0;

	save_msg(message_key, msg);
    }

    saved_msg = 0;
    return msg;
}


private nomask int get_timestamp()
{
    int timestamp;

    timestamp = time();
    while ( unguarded(1, (: is_file, get_fname(timestamp) + __SAVE_EXTENSION__ :)) )
	timestamp--;
    return timestamp;
}

private nomask int user_exists(string user)
{
    return unguarded(1, (: is_file, USER_PATH(user) + __SAVE_EXTENSION__ :));
}

private nomask void deliver_mail(int timestamp, string who)
{
    MAILBOX_D->get_mailbox(who)->receive_new_message(timestamp);
}

nomask string * process_mail_list(string * list)
{
    /* convert groups to real names, then clean the lists: remove dups,
       ensure lower-cased, ensure valid */
    list = GROUP_D->process_list(list);
    list = clean_array(list);
    list = map_array(list, (: lower_case :));
    list = filter_array(list, (: user_exists :));

    return list;
}

nomask string * send_mail(string 	Sender,
			  string	Subject,
			  string*	Body,
			  string*	To_list,
			  string*	Cc_list)
{
    class mail_msg msg;
    int		timestamp;
    string*	recip_list;

    // No mail forgeries, except by system stuff =-)
    // (this is so that system objects can send mail as Root or whatever)
    if ( !check_privilege(1) &&
	 ( !this_user() || this_user()->query_userid() != Sender ) )
	error("insufficient priviledge to send mail as " + Sender + "\n");

    msg = new(class mail_msg);
    msg->to_list = To_list;
    msg->cc_list = Cc_list;
    msg->sender = Sender;
    msg->subject = Subject;
    msg->body = Body;

    if( !pointerp(msg->to_list) || !pointerp(msg->cc_list) )
	error("send mail: invalid list of recipients");

    if( !pointerp(msg->body) )
	msg->body = ({ "**Blank message!**" });

    /* Convert groups to real names */
    msg->to_list = process_mail_list(msg->to_list);
    msg->cc_list = process_mail_list(msg->cc_list);

    // this is the mail pointer as well as within seconds of time()
    timestamp = get_timestamp();
    msg->date = msg->thread_id = timestamp;

    // A list of target recipients with no names duplicated
    recip_list = clean_array(msg->to_list + msg->cc_list);

    // deliver the mail (the message keys) to all recipients
    map_array(recip_list, (: deliver_mail, timestamp :));

    // we need to keep track of who needs to delete this message before
    // we erase this message to prevent people from making a call
    // to delete a mail into oblivion.
    msg->dels_pending = recip_list;

    // If there are no active copies, no one is receiving this message,
    // so why save it?
    if ( sizeof(recip_list) )
	save_msg(timestamp, msg);

    // close the mailboxes of people who are not online now
    MAILBOX_D->close_mailboxes();

    // Let the mailer know who successfully received the message.
    return recip_list;
}


nomask mixed get_one_message(int timestamp)
{
    class mail_msg msg;

    if ( base_name(previous_object()) != MAILBOX )
	error("security violation: illegal attempt to read mail\n");

    if ( !(msg = restore_msg(timestamp)) )
	error("lost the message\n");

    if ( member_array(this_user()->query_userid(), msg->dels_pending) == -1 )
	error("security violation: illegal attempt to read mail\n");

    return msg;
}

nomask void delete_mail(int timestamp, string user)
{
    class mail_msg msg;

    // Do I have permission to delete mail as user?
    // Should only fail when I try to delete mail in someone elses box
    if ( base_name(previous_object()) != MAILBOX ||
	 previous_object()->query_owner() != user )
	error("security violation: illegal attempt to delete mail\n");

    if ( !(msg = restore_msg(timestamp)) )
	error("lost the message\n");

    msg->dels_pending -= ({ user });
    if( !sizeof(msg->dels_pending) )
	unguarded(1, (: rm, get_fname(timestamp) + __SAVE_EXTENSION__ :));
    else
	save_msg(timestamp, msg);
}

private nomask void process_message(string fname)
{
    int message_key;
    class mail_msg msg;

    message_key = to_int(fname[0..<3]);	/* ### dependent on SAVE_EXTENSION */
    msg = restore_msg(message_key);
    map(msg->dels_pending,
	(: MAILBOX_D->get_mailbox($1)->receive_new_message($(message_key)) :));
}

nomask void rebuild_mailboxes()
{
    string * messages = unguarded(1, (: get_dir, MAIL_PATH "*.o" :));

    map_array(messages, (: process_message :));
}


//### TEMPORARY for a bit...
nomask object get_mailbox(string the_owner)
{
    return MAILBOX_D->get_mailbox(the_owner);
}
nomask void unload_mailbox(string the_owner)
{
    MAILBOX_D->unload_mailbox(the_owner);
}
nomask void close_mailboxes()
{
    MAILBOX_D->close_mailboxes();
}
