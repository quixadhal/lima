/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mailbox.c -- a mailbox to hold message references
**
** 950701, Deathblade: Created.
*/

#include <mudlib.h>
#include <security.h>
#include <mail.h>
#include <flags.h>
#include <playerflags.h>

inherit M_ACCESS;
/* ### inherit DAEMON? */

/*
** mailbox: maps message keys to flags
*/
private mapping mailbox = ([ ]);

/*
** Flags for the mail
*/
#define F_UNREAD	1
/* ### code assumes zero/non-zero; fix if more flags are added */

/*
** Who is this mailbox for?
*/
private static string owner;

/*
** What is the current message index.  This value is 0-based and represents
** a message index for messages from the user's perspective.  The value 1
** corresponds to the first key in the (ordered) set of message keys in the
** mailbox.
*/
private static int message_index = 0;


private nomask string get_fname()
{
    return sprintf("/data/mail/mbox/%c/%s", owner[0], owner);
}

nomask void create(string the_owner)
{
    if ( !clonep() )
	return;

    if ( file_name(previous_object()) != MAIL_D )
	error("*Security violation: invalid creation");

    owner = the_owner;
    unguarded(1, (: restore_object, get_fname(), 1 :));
}
private nomask void save_me()
{
    unguarded(1, (: save_object, get_fname() :));
}
nomask void remove()
{
    destruct(this_object());
}

nomask string query_owner()
{
  return owner;
}

nomask class mail_msg get_one_message(int message_key)
{
    mixed * hdrs;
    string * body;
    class mail_msg msg;

    printf("prev=%O\n",previous_object(-1));
//    if ( !check_privilege(owner) )
//	error("*Security violation: you are not allowed to read this mail");

    if ( undefinedp(mailbox[message_key]) )
	error("*Security violation: that message is not in this mailbox");

    hdrs = MAIL_D->get_headers(({ message_key }));
    body = MAIL_D->get_body(message_key);

    msg = new(class mail_msg);
    msg->to_list	= hdrs[0][2];
    msg->cc_list	= hdrs[0][3];
    msg->from		= hdrs[0][0];
    msg->date		= message_key;
    msg->subject	= hdrs[0][1];
    msg->body		= body;

    return msg;
}

nomask int query_message_index()
{
    return message_index;
}
nomask void set_message_index(int new_message_index)
{
    message_index = new_message_index;
}

nomask int * query_message_keys()
{
    return sort_array(keys(mailbox), 1);
}
nomask int query_message_count()
{
    return sizeof(mailbox);
}

nomask int query_message_read(int message_key)
{
    return mailbox[message_key];
}
nomask void set_message_read(int message_key)
{
    if ( mailbox[message_key] )
    {
	mailbox[message_key] = 0;
	save_me();
    }
}
nomask int first_unread_message()
{
    int * message_keys = query_message_keys();
    int message_key;
    int message_index;

    foreach ( message_key in message_keys )
    {
	if ( mailbox[message_key] )
	    return message_index;

	++message_index;
    }

    return -1;
}

nomask void delete_message(int message_key)
{
    MAIL_D->delete_mail(message_key, owner);
    map_delete(mailbox, message_key);

    save_me();
}

/* ### temp functions?  wait and see */

nomask void receive_new_message(int message_key)
{
    object user;

//    if( previous_object() != find_object( MAIL_D ) )
//	return;

    mailbox[message_key] = F_UNREAD;
    save_me();

    if ( (user = find_user(owner)) &&
	 user->query_body()->test_flag(F_BIFF) )
	tell_object(user, ">>You have new mail<<\n");
}
