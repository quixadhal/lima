/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mailer.c
**
** Base class for a mailer object.  Provides basic support for all mailers.
**
** Created 31-OCT-94, Deathblade.
*/

#include <security.h>
#include <daemons.h>
#include <playerflags.h>
#include <mudlib.h>
#include <classes.h>

inherit M_ACCESS;
inherit M_INPUT;

inherit CLASS_MAILMSG;

//### make it private so it can't be changed?
static object	mailbox_ob;


/*
** tmp_fname()
**
** Return a temporary filename for editing mail messages
*/
static nomask string tmp_fname()
{
    return "/tmp/tmail." + this_user()->query_userid();
}

/*
** get_message_key()
**
** Get a message key from a user's message number.
** 0 is returned if the user number is out of bounds.
*/
static nomask int get_message_key(int user_num)
{
    int * mail_keys;

    if ( !user_num )
	user_num = mailbox_ob->query_message_index();
    else 
	user_num--;

    mail_keys = mailbox_ob->query_message_keys();
    if ( user_num < 0 || user_num >= sizeof(mail_keys) )
    {
	write("Message number out of bounds.\n");
	return 0;
    }

    return mail_keys[user_num];
}

/*
** format_name_list()
**
** Format a list of names (as in To: or CC:).  Returns an array of lines.
*/
static nomask string * format_name_list(string prompt, string * names)
{
    if ( !names || !sizeof(names) )
	return ({ });

    return explode(iwrap(prompt + implode(map_array(names, (: capitalize :)),
					  ", "),
			 strlen(prompt)),
		   "\n");
}

/*
** build_message()
**
** Build an array of strings containing a mail message.
*/
static nomask string * build_message(int mail_key, int supress_header)
{
    string *	output;
    class mail_msg msg;

    msg = mailbox_ob->get_one_message(mail_key);

    output = ({});
    if ( !supress_header )
    {
	output += format_name_list("To     : ", msg->to_list);
	output += format_name_list("Cc     : ", msg->cc_list);
	output += ({ "From   : " + capitalize(msg->sender) });
	output += ({ "Date   : " + ctime(msg->date) });
	output += explode(iwrap("Subject: " + msg->subject),"\n");
	output += ({ sprintf("%'-'39s","-") });
    }

    output += msg->body;
    output += ({ "" });

    return output;
}

/*
** write_dead_letter()
**
** Write the array of lines to the wizard's dead.letter.  Does nothing
** if the player is not a wizard or no home dir exists.
*/
static nomask void write_dead_letter(string * buf)
{
    if ( wizardp(this_user()) && 
	file_size("/wiz/"+this_user()->query_userid()) == -2 )
    {
	write("Appending to ~/dead.letter\n");
	write_file(sprintf("/wiz/%s/dead.letter",
			   this_user()->query_userid()),
		   implode(buf,"\n")+"\n");
    }
}

/*
** build_body_inclusion()
**
** Build an array of lines for the body of a message to be included into
** another message (prefixed with "> ")
*/
static nomask string * build_body_inclusion(string * body)
{
    return map_array(body, (: "> " + $1 :));
}

/*
** send_mail_message()
**
** Send a mail message to the given people.
*/
private nomask void send_mail_message(string subject,
				    string *buf,
				    mixed to_list,
				    mixed cc_list,
				    int use_dead_letter)
{
    string * name_list;

    if ( stringp(to_list) )
	to_list = explode(to_list, " ");
    else if ( !pointerp(to_list) )
	to_list = ({ });

    if ( !sizeof(to_list) )
    {
	write("No destination names.\n");
	if ( use_dead_letter )
	    write_dead_letter(buf);
	return;
    }
    
    if ( stringp(cc_list) )
	cc_list = explode(cc_list, " ");
    else if ( !pointerp(cc_list) )
	cc_list = ({ });

    name_list = MAIL_D->send_mail(this_user()->query_userid(),
				  subject,
				  buf,
				  to_list,
				  cc_list);
    if ( !sizeof(name_list) )
    {
	write("No valid destination.\n");
	if ( use_dead_letter )
	    write_dead_letter(buf);
	return;
    }

    write(implode(format_name_list("Mail sent to: ", name_list), "\n") + "\n");
}


/*------------------------------------------------------------------------
**
** COMMANDS
**
** These are some basic implementations of mail commands.  Mailer
** subclasses may use them if they fit within the schema/style of
** the mailer interface.  If not, then subclasses are, of course,
** free to implement new commands.
**
** Note that the commands must have their arguments validated on
** entry -- usage information cannot be printed from these commands.
*/

static nomask void cmd_read(int user_num,
			    string outputfile,
			    int supress_header)
{
    int		timestamp;
    string *	output;

    if ( !user_num )
	user_num = mailbox_ob->query_message_index() + 1;

    timestamp = get_message_key(user_num);
    if ( !timestamp ) return;

    output = build_message(timestamp, supress_header);

    if ( outputfile )
    {
	if ( strsrch(outputfile,"/") == -1 )
	    outputfile = wiz_dir(this_user()) + "/Mail/" + outputfile;

	if ( !write_file(outputfile, implode(output,"\n")) )
	    write("Failed.\n");
	else
	    printf("Saved message to %s.\n",outputfile);
	return;
    }

    mailbox_ob->set_message_index(user_num - 1);
    mailbox_ob->set_message_read(timestamp);

    this_user()->more(output);
}


static nomask void cmd_headers(string rangestr)
{
    int   i,j;
    string* output;
    int * mail_keys;
    mapping nums;
    int key;

    output = ({});

    if ( rangestr )
	rangestr = expand_range(rangestr);

    mail_keys = mailbox_ob->query_message_keys();
    if ( !sizeof(mail_keys) )
	return write("No mail.\n");

    nums = ([ ]);
    for ( i = sizeof(mail_keys); i--; )
	nums[mail_keys[i]] = i + 1;

    if ( rangestr )
    {
	for ( i = sizeof(mail_keys); i--; )
	    if ( !test_bit(rangestr, i+1) )
	    {
		nums[mail_keys[i]] = 0;
		mail_keys[i] = 0;
	    }

	mail_keys -= ({ 0 });
    }

    foreach ( key in mail_keys )
    {
	class mail_msg msg = mailbox_ob->get_one_message(key);

	output +=
	    ({ sprintf("%c %-3d %-15s (%s) %s",
		       mailbox_ob->query_message_read(key) ? 'N' : ' ',
		       nums[key],
		       capitalize(msg->sender),
		       ctime(msg->date)[0..9],
		       msg->subject)
		   });
    }

    this_user()->more(output);
}


private nomask void mailer_get_subject(string to_list, string arg)
{
    string subject = arg ? arg : "<none>";

    clone_object(EDIT_OB)->edit_file(tmp_fname(),
				     "mailer_done_edit",
				     ({ to_list, subject }));
}

static nomask void cmd_mail(string to_list)
{
    //Until a new maskable editor is in place, don't allow a null
    // to line.
    if ( !stringp(to_list) )
    {
	write("No destination.\n");
	return;
    }

    write("Subject: ");
    modal_simple((: mailer_get_subject, to_list :));
}


/* to_list is a string * or a string */
nomask void mailer_get_cc_list(mixed to_list, string subject, string cc_list)
{
    string * buf;
    string file;

    file = tmp_fname();
    buf = explode(read_file(file), "\n");
    send_mail_message(subject, buf, to_list, cc_list, 1);

    rm(file);
}

nomask void mailer_done_edit(mixed ctx, string fname)
{
    /*
    ** Just return if they cancelled the edit
    */
    if ( !fname )
	return;

    write("Cc: ");
    modal_simple((: mailer_get_cc_list, ctx[0], ctx[1] :));
}


static nomask void cmd_reply(int user_num, int reply_all)
{
    int key;
    class mail_msg msg;
    mixed body;
    string * to_list;
    string subject;
    string file;

    key = get_message_key(user_num);
    if ( !key )
	return;

    msg = mailbox_ob->get_one_message(key);

    body = build_body_inclusion(msg->body);
    body = implode(body, "\n");
    body = sprintf("On %s %s wrote:\n%s\n",
		   ctime(msg->date),
		   capitalize(msg->sender),
		   body);

    subject = "Re: " + msg->subject;

    to_list = ({ msg->sender });
    if ( reply_all )
	to_list = clean_array(to_list + msg->to_list + msg->cc_list);

    file = tmp_fname();
    write_file(file, body);
    clone_object(EDIT_OB)->edit_file(file,
				     "mailer_done_edit",
				     ({ to_list, subject }));
}


static nomask void cmd_delete(string arg)
{
    int *	mail_keys;
    int		i;
    string	whichones;

    mail_keys = mailbox_ob->query_message_keys();

    whichones = "";
    if( arg )
	whichones = expand_range( arg );
    else
	whichones = set_bit(whichones, mailbox_ob->query_message_index() + 1);

    for ( i = sizeof(mail_keys) + 1; --i; )
	if( test_bit(whichones,i) )
	{
	    mailbox_ob->delete_message(mail_keys[i-1]);
	    printf("Message %d deleted.\n",i);
	}
}


static nomask void cmd_setcurrent(mixed arg)
{
    int count;

    if ( !(count = mailbox_ob->query_message_count()) )
	return printf("No messages.\n");

    if ( !arg )
	return (void)printf("Current message is: %d\n",
			    mailbox_ob->query_message_index() + 1);

    arg = to_int( arg );
    if ( arg <= 0 || arg > count )
    {
	printf("Message number out of range.\n");
	return;
    }
    mailbox_ob->set_message_index(arg - 1);

    printf("Current message now set to: %d\n",arg);
}


static nomask void cmd_forward(int user_num, string newto)
{
    int			key;
    string *		body;
    class mail_msg	msg;

    key = get_message_key(user_num);
    if ( !key ) return;

    msg = mailbox_ob->get_one_message(key);

    body = build_body_inclusion(msg->body);
    body = ({"Begin forwarded message:",
		 sprintf("On %s %s wrote:",
			 ctime(msg->date),
			 capitalize(msg->sender))
	 }) + body;

    send_mail_message("FWD: " + msg->subject,
		      body,
		      newto,
		      ({ }),	/* cc_list */
		      0);
}

void create()
{
    set_privilege(1);

    mailbox_ob = MAIL_D->get_mailbox(this_user()->query_userid());
}

void remove()
{
    destruct(this_object());
}
