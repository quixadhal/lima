/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz on July 16 for Zorkmud, something that any mailer can
// interface with, so Beek and I can have our input_to()less mailer.
//
// 950710, Deathblade: Added mailboxes, removed mail queue.

#include <mudlib.h>

inherit DAEMON;


#define MAIL_PATH	"/data/M/"

string	sender;
string  subject;
string* body;
string* to_list;
string* cc_list;
string* dels_pending;

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
	    mailboxes[name]->remove();
	    map_delete(mailboxes, name);
	}
}

private nomask void process_message(string fname)
{
    unguarded(1, (: restore_object, "/data/M/" + fname[0..<3], 1 :));
    map_array(dels_pending,
	      (: get_mailbox($1)->receive_new_message($(to_int(fname))) :));
}

nomask void rebuild_mailboxes()
{
    string * messages = unguarded(1, (: get_dir, "/data/M/*.o" :));

    map_array(messages, (: process_message :));
    voidout();
}

private
int
restore_msg( mixed timestamp )
{
  timestamp = MAIL_PATH + timestamp;
  return unguarded( 1, (: restore_object, timestamp, 1 :) );
}

private
int
valid_read()
{
   string myname;

  myname = this_body()->query_name();
  return member_array( myname, dels_pending );
}


private
void
save_msg( int timestamp )
{
  string	savefile;

  savefile = MAIL_PATH + timestamp;
  unguarded( 1, (: save_object, savefile :) );
}


private
int
get_timestamp()
{
  int		timestamp;
  string	file;

  timestamp = time();
  while( ( file = MAIL_PATH + timestamp + ".o" ) && 
         unguarded(1, (: file_size, file :)) != -1 )
		timestamp--;
  return timestamp;
}

private
int
confirm( string who )
{
  if(get_privilege( previous_object() ) == 1 ) 
    return 1;
  if( !this_user() )
    return 0;


  return (string)this_user()->query_userid() == who;
}

private nomask int user_exists(string user)
{
    return unguarded(1, (: file_size, USER_PATH(user)+".o" :)) > -1;
}

private nomask void deliver_mail(int timestamp, string who)
{
    get_mailbox(who)->receive_new_message(timestamp);
}

string*
send_mail( string 	Sender,
	   string	Subject,
	   string*	Body,
	   string*	To_list,
	   string*	Cc_list )
{
  int		timestamp;
  string*	recip_list;
  string*	badtargs;

// No mail forgeries, except by system stuff =-)
// (this is so that system objects can send mail as Root or whatever)
  if( !confirm( Sender ) )
  {
    error( "Insufficient privlidge to send mail as "+Sender );
    return 0;
  }
  
  sender = Sender;
  subject = Subject;
  body = Body;
  to_list = To_list;
  cc_list = Cc_list;

  if( !pointerp(to_list) )
  {
    error( "send mail: invalid target" );
    return 0;
  }
  if( !pointerp( to_list ) )
    to_list = ({});

  if( !pointerp( cc_list ) )
    cc_list = ({});

  if( !pointerp( body ) )
    body = ({ "**Blank message!**" });


// Convert groups to real names
  to_list = GROUP_D->process_list(to_list);
  cc_list = GROUP_D->process_list(cc_list);

  to_list = map_array( to_list, (: lower_case :) );
  cc_list = map_array( cc_list, (: lower_case :) );

// this is the mail pointer as well as within seconds of time()
  timestamp = get_timestamp();

// A list of target recipients with no names duplicated
  recip_list = clean_array( cc_list + to_list );

  // filter out non-existent users
  recip_list = filter_array( recip_list, (: user_exists :) );

  // deliver the mail to all users
  map_array(recip_list, (: deliver_mail, timestamp :));

// Figure out if there were any bogus names in our target group
  badtargs = clean_array( cc_list + to_list ) - recip_list;

// If so, delete them from the cc and to list.
// Also, remove duplicate entries of the same name in these lists.
  cc_list = clean_array( cc_list - badtargs );
  to_list = clean_array( to_list - badtargs );


// we need to keep track of who needs to delete this message before
// we erase this message to prevent people from making a call
// to delete a mail into oblivion.

  dels_pending = recip_list;

// If there are no active copies, no one is receiving this message,
// so why save it?
  if( sizeof( recip_list ) )
    save_msg( timestamp );

  voidout();

  // close the mailboxes of people who are not online now
  close_mailboxes();

// Let the mailer know who successfully received the message.
  return recip_list;

}


// Maps an array of timestamps to an array of arrays containing
// the header info for each timestamp, plus the timestamp
// appended to the end in ctime() notation.
// timestamps for mails that don't exist are ignored.

mixed*
get_headers( int* timestamps )
{
  string*	ret;
  int		i,j;

  ret = ({});

  if( !pointerp( timestamps ) )
    return ({});

  j = sizeof( timestamps );

  for( ; i < j ; i++ )
  {
  if( !restore_msg( timestamps[i] ) )
	continue;

  if( !valid_read() )
        continue;
    ret += ({ ({ sender, subject, to_list, cc_list, ctime( timestamps[i] ) }) });
  }
  voidout();
  return ret;
}

// I figure you only want to read one body at a time.  If you need
// more, map_array to a symbol_function of this.

mixed
get_body( int timestamp )
{
  if( !restore_msg( timestamp ) )
    return -1;

  if( !valid_read() )
    return 0;

  call_out("voidout",0);
  return body;
}

int
delete_mail( int timestamp, string user )
{
  string	savefile;

// Do I have permission to delete mail as user?
// Should only fail when I try to delete mail in someone elses box
  if( !confirm( user ) )
    return 0;

  if( !restore_msg( timestamp ) )
    return -1;

  savefile = MAIL_PATH + timestamp;

  
  if( member_array( user, dels_pending ) == -1 )
    return 0;

  dels_pending -= ({user});
  if( !sizeof( dels_pending ) )
    unguarded( 1, (: rm, savefile+".o" :) );
  else
    save_msg( timestamp );

  voidout();
  return 1;
}

// Whoops, I really shouldn't have sent that mail.
int
recall_msg( int timestamp, string uid )
{

  string	savefile;

  if( !restore_msg( timestamp ) )
    return -1;

  if( uid != sender )
    return 0;

  savefile = MAIL_PATH + timestamp;

  unguarded( 1, (: rm, savefile :) );
  voidout();
  return 1;
}
