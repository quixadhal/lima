/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Belboz on July 16 for Zorkmud, something that any mailer can
** interface with, so Beek and I can have our input_to()less mailer.
**
** 950710, Deathblade: Added mailboxes, removed mail queue.
** 950910, Deathblade: revamped some more: doc, trim api, cleaning
**
** Public API:
**
**   send_mail(): send a piece of mail
**   get_mailbox(): return a MAILBOX object for the specified user
**   unload_mailbox(): the specified mailbox is no longer needed; close it
**   close_mailboxes(): close all unneeded mailboxes
**   rebuild_mailboxes(): rebuild all mailbox from actual message data
**   delete_mail(): delete a piece of mail for a user
**   get_one_message(): get a message
*/

#include <mudlib.h>
#include <classes.h>

inherit DAEMON;
inherit CLASS_MAILMSG;

#define MAIL_PATH	"/data/M/"

private string   sender;
private string   subject;
private string * body;
private string * to_list;
private string * cc_list;
private string * dels_pending;

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

private nomask void voidout()
{
    sender = 0;
    subject = 0;
    body = 0;
    to_list = 0;
    cc_list = 0;
}

nomask object get_mailbox(string the_owner)
{
    object mailbox;

    if ( !(mailbox = mailboxes[the_owner]) )
    {
	mailbox = mailboxes[the_owner] = new(MAILBOX, the_owner);
	if ( !mailbox )
	    error("mailbox wasn't created");
    }

    return mailbox;
}

nomask void unload_mailbox(string the_owner)
{
    object mailbox;

    if ( mailbox = mailboxes[the_owner] )
    {
        mailbox->remove();
        map_delete(mailboxes, the_owner);
    }
}

nomask void close_mailboxes()
{
    string * names;
    string name;

    names = keys(mailboxes) - users()->query_userid();
    foreach ( name in names )
    {
	if ( mailboxes[name] )	/* might be have destructed */
	    mailboxes[name]->remove();
	map_delete(mailboxes, name);
    }
}

private nomask void process_message(string fname)
{
    unguarded(1, (: restore_object, MAIL_PATH + fname[0..<3], 1 :));
    map_array(dels_pending,
	      (: get_mailbox($1)->receive_new_message($(to_int(fname))) :));
}

nomask void rebuild_mailboxes()
{
    string * messages = unguarded(1, (: get_dir, MAIL_PATH "*.o" :));

    map_array(messages, (: process_message :));
    voidout();
}


private nomask string get_fname(int message_key)
{
    /* ### eventually move this to /data/mail/N/N/NNNNNNNN.o */
    return sprintf(MAIL_PATH "%d", message_key);
}

private nomask int restore_msg(int message_key)
{
    return unguarded(1, (: restore_object, get_fname(message_key), 1 :));
}
private nomask void save_msg(int message_key)
{
    unguarded(1, (: save_object, get_fname(message_key) :));
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
    get_mailbox(who)->receive_new_message(timestamp);
}

nomask string * send_mail(string 	Sender,
			  string	Subject,
			  string*	Body,
			  string*	To_list,
			  string*	Cc_list)
{
    int		timestamp;
    string*	recip_list;

    // No mail forgeries, except by system stuff =-)
    // (this is so that system objects can send mail as Root or whatever)
    if ( !check_privilege(1) &&
	 ( !this_user() || this_user()->query_userid() != Sender ) )
	error("insufficient priviledge to send mail as " + Sender + "\n");

    sender = Sender;
    subject = Subject;
    body = Body;
    to_list = To_list;
    cc_list = Cc_list;

    if( !pointerp(to_list) || !pointerp(cc_list) )
	error("send mail: invalid list of recipients");

    if( !pointerp( body ) )
	body = ({ "**Blank message!**" });

    /* Convert groups to real names */
    to_list = GROUP_D->process_list(to_list);
    cc_list = GROUP_D->process_list(cc_list);

    /* clean the lists: remove dups, ensure lower-cased, ensure valid */
    to_list = clean_array(to_list);
    cc_list = clean_array(cc_list);

    to_list = map_array(to_list, (: lower_case :));
    cc_list = map_array(cc_list, (: lower_case :));

    to_list = filter_array(to_list, (: user_exists :));
    cc_list = filter_array(cc_list, (: user_exists :));

    // this is the mail pointer as well as within seconds of time()
    timestamp = get_timestamp();

    // A list of target recipients with no names duplicated
    recip_list = clean_array(cc_list + to_list);

    // deliver the mail to all users
    map_array(recip_list, (: deliver_mail, timestamp :));

    // we need to keep track of who needs to delete this message before
    // we erase this message to prevent people from making a call
    // to delete a mail into oblivion.
    dels_pending = recip_list;

    // If there are no active copies, no one is receiving this message,
    // so why save it?
    if ( sizeof(recip_list) )
	save_msg(timestamp);

    voidout();

    // close the mailboxes of people who are not online now
    close_mailboxes();

    // Let the mailer know who successfully received the message.
    return recip_list;
}


nomask mixed get_one_message(int timestamp)
{
    class mail_msg msg;

    if ( base_name(previous_object()) != MAILBOX )
	error("security violation: illegal attempt to read mail\n");

    if ( !restore_msg(timestamp) )
	error("lost the message\n");

    if ( member_array(this_user()->query_userid(), dels_pending) == -1 )
	error("security violation: illegal attempt to read mail\n");

    msg = new(class mail_msg);
    msg->to_list	= to_list;
    msg->cc_list	= cc_list;
    msg->sender		= sender;
    msg->date		= timestamp;
    msg->subject	= subject;
    msg->body		= body;
    msg->dels_pending	= dels_pending;

    voidout();

    return msg;
}

nomask void delete_mail(int timestamp, string user)
{
    // Do I have permission to delete mail as user?
    // Should only fail when I try to delete mail in someone elses box
    if ( base_name(previous_object()) != MAILBOX ||
	 previous_object()->query_owner() != user )
	error("security violation: illegal attempt to delete mail\n");

    if ( !restore_msg(timestamp) )
	error("lost the message\n");

    dels_pending -= ({ user });
    if( !sizeof(dels_pending) )
	unguarded(1, (: rm, get_fname(timestamp) + __SAVE_EXTENSION__ :));
    else
	save_msg(timestamp);

    voidout();
}
