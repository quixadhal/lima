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
#include <log.h>

inherit M_ACCESS;
inherit CLASS_MAILMSG;

#define MAIL_PATH	"/data/M/"

private class mail_msg saved_msg;

private static mapping mailboxes = ([ ]);


private nomask void create()
{
    object mailbox;

    set_privilege(1);
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

private nomask class mail_msg restore_msg(int message_key)
{
    class mail_msg msg;

    if ( !unguarded(1, (: restore_object, get_fname(message_key), 1 :)) )
	return 0;

    msg = saved_msg;

    /* don't leave the message sitting around in our variables */
    saved_msg = 0;

    return msg;
}

private nomask int get_message_key()
{
    int message_key;

    message_key = time();
    while ( unguarded(1, (: is_file, get_fname(message_key) + __SAVE_EXTENSION__ :)) )
	message_key--;
    return message_key;
}

private nomask void deliver_mail(int message_key, string who)
{
    MAILBOX_D->get_mailbox(who)->receive_new_message(message_key);
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

private nomask mixed clean_addresses(string array list)
{
    string 	user, mudname;
    mixed 	mudinf;
    string	array local_recips = ({});
    string	array recips = ({});
    string	listitem;

    foreach(listitem in list)
    {
	listitem = lower_case( listitem );
	if(!sscanf(listitem, "%s@%s", user, mudname))
	{
	    if(user_exists(listitem))
	    {
		local_recips += ({listitem});
		recips += ({listitem});
	    }
	    continue;
	}
	mudinf = IMAIL_D->get_complete_mudname(mudname);
	if(arrayp(mudinf))
	{
	    recips += ({listitem});
	    continue;
	}
	if(mudinf == mud_name())
	{
	    recips += ({user});
	    local_recips += ({user});
	    continue;
	}
	recips += ({user + "@" + mudinf});
    }
    return ({local_recips, recips});
}

string array ungroup( string array list )
{
    mixed to;

    foreach( string addressee in list )
    {
	to = GROUP_D->get_group( addressee);
	if( !arrayp( to )) to = ({ to });
	list -= ({ addressee });
	list += to;
    }
    return list;
}


varargs nomask string * send_mail(string 	Sender,
  string	Subject,
  mixed		Body,
  string*	To_list,
  string*	Cc_list,
  // Send time should only be provided by
  // the imud mail stuff...
  int		send_time)
{
    class mail_msg msg;
    int		message_key;
    string*	recip_list;
    string*	local_recip_list;
    mixed	recip_lists;

    // No mail forgeries, except by system stuff =-)
    // (this is so that system objects can send mail as Root or whatever)
    if ( !check_previous_privilege(1) &&
      ( !this_user() || this_user()->query_userid() != Sender ) )
	error("insufficient priviledge to send mail as " + Sender + "\n");

    if ( stringp(Body) )
	Body = explode(Body, "\n");
    else if ( !pointerp(Body) )
	Body = ({ "**Blank message!**" });

    msg = new(class mail_msg);
    msg->to_list = To_list;
    msg->cc_list = Cc_list;
    msg->sender = Sender;
    msg->subject = Subject;
    msg->body = Body;

    if ( !pointerp(msg->to_list) || !pointerp(msg->cc_list) )
	error("send mail: invalid list of recipients");

    /* 
    ** Pass it to the IMAIL_D for a second, let it see
    ** if any of the addresses are imud addresses.
    */

    /* Convert groups to real names */
    To_list = ungroup( To_list );
    Cc_list = ungroup( Cc_list );

    // this is the mail pointer as well as within seconds of time()
    message_key = get_message_key();
    msg->date = msg->thread_id = message_key;
    if ( send_time )
    {
	msg->date = send_time;
    }

    // A list of target recipients with no names duplicated
 Cc_list -= To_list;
    recip_lists = clean_addresses( To_list + Cc_list);
    local_recip_list = recip_lists[0];
    recip_list       = recip_lists[1];
    local_recip_list = process_mail_list(local_recip_list);

    msg->to_list = recip_list;
    // deliver the mail (the message keys) to all recipients
    map_array(local_recip_list, (: deliver_mail, message_key :));

    // we need to keep track of who needs to delete this message before
    // we erase this message to prevent people from making a call
    // to delete a mail into oblivion.
    msg->dels_pending = local_recip_list;

    // If there are no active copies, no one on this mud 
    // is receiving this message, so why save it? 
    if ( sizeof(local_recip_list) )
	save_msg(message_key, msg);

    // close the mailboxes of people who are not online now
    MAILBOX_D->close_mailboxes();

    if ( previous_object() != find_object(IMAIL_D) && 
      sizeof(recip_list) != sizeof(local_recip_list) )
    {
	IMAIL_D->enqueue_message(msg);
    }

    // Let the mailer know who successfully received the message.
    return recip_list;
}


nomask mixed get_one_message(int message_key)
{
    class mail_msg msg;

    if ( base_name(previous_object()) != MAILBOX )
	error("security violation: illegal attempt to read mail\n");

    if ( !(msg = restore_msg(message_key)) )
    {
	LOG_D->log(LOG_MAIL_ERROR,
	  sprintf("%s lost message #%d",
	    this_user()->query_userid(),
	    message_key));
	return 0;
    }

    if ( member_array(this_user()->query_userid(), msg->dels_pending) == -1 )
	error("security violation: illegal attempt to read mail\n");

    return msg;
}

nomask void delete_mail(int message_key, string user)
{
    class mail_msg msg;

    // Do I have permission to delete mail as user?
    // Should only fail when I try to delete mail in someone elses box
    if ( base_name(previous_object()) != MAILBOX ||
      previous_object()->query_owner() != user )
	error("security violation: illegal attempt to delete mail\n");

    if ( !(msg = restore_msg(message_key)) )
	error("lost the message\n");

    msg->dels_pending -= ({ user });
    if( !sizeof(msg->dels_pending) )
	unguarded(1, (: rm, get_fname(message_key) + __SAVE_EXTENSION__ :));
    else
	save_msg(message_key, msg);
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
