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
#include <mail.h>

inherit M_ACCESS;
inherit M_INPUT;

/* ### make it private so it can't be changed? */
static object	mailbox_ob;

private static mixed	to_list;
private static string	subject;


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

    output = ({});
    if ( !supress_header )
    {
	mixed	headers;

	headers = MAIL_D->get_headers( ({ mail_key }) )[0];

	output += format_name_list("To     : ", headers[2]);
	output += format_name_list("Cc     : ", headers[3]);
	output += ({ "From   : " + capitalize(headers[0]) });
	output += ({ "Date   : " + headers[4] });
	output += explode(iwrap("Subject: " + headers[1]),"\n");
	output += ({ sprintf("%'-'39s","-") });
    }

    output += MAIL_D->get_body(mail_key);
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
static nomask string * build_body_inclusion(int mail_key)
{
    return map_array(MAIL_D->get_body(mail_key), (: "> " + $1 :) );
}

/*
** send_mail_message()
**
** Send a mail message to the given people.
*/
static nomask int send_mail_message(string subject,
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
	return 0;
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
	return 0;
    }

    write(implode(format_name_list("Mail sent to: ", name_list), "\n") + "\n");

    return 1;
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

    clone_object(MORE_OB)->more_string(output);
}


static nomask void cmd_headers(string rangestr)
{
    int   i,j;
    string* output;
    int * mail_keys;
    mapping nums;
    mixed * headers;

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

    headers = MAIL_D->get_headers(mail_keys);
    if ( sizeof(headers) != sizeof(mail_keys) )
	return write("Bad range\n");

    i = sizeof(headers);
    for ( ; j < i; j++ )
	output +=
	({ sprintf("%c %-3d %-15s (%s) %s",
	    mailbox_ob->query_message_read(mail_keys[j]) ? 'N' : ' ',
	    nums[mail_keys[j]],
	    capitalize(headers[j][0]),
	    headers[j][4][0..9],
	    headers[j][1]
	  )
	});

    clone_object(MORE_OB)->more_string(output);
}


private nomask void mailer_get_subject(string arg)
{
    subject = arg ? arg : "<none>";

    clone_object(EDIT_OB)->edit_file(tmp_fname(), "mailer_done_edit", 0);
}

static nomask void cmd_mail(string arg)
{
    //Until a new maskable editor is in place, don't allow a null
    // to line.
    if ( !stringp(arg) )
    {
	write("No destination.\n");
	return;
    }

    to_list = arg;
    write("Subject: ");
    modal_simple((: mailer_get_subject :));
}



nomask void mailer_get_cc_list(string cc_list)
{
    string * buf;
    string file;

    file = tmp_fname();
    buf = explode(read_file(file), "\n");
    (void)send_mail_message(subject, buf, to_list, cc_list, 1);

    to_list = 0;
    subject = 0;
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
    modal_simple((: mailer_get_cc_list :));
}


static nomask void cmd_reply(int user_num, int reply_all)
{
    mixed body;
    mixed replyh;
    string file;
    int key;

    key = get_message_key(user_num);
    if ( !key ) return;

    replyh = MAIL_D->get_headers( ({ key }) );
    replyh = replyh[0];
    body = build_body_inclusion(key);
    body = implode(body, "\n");
    body = sprintf("On %s %s wrote:\n%s\n",
		   replyh[4],
		   capitalize(replyh[0]),
		   body);

    subject = "Re: " + replyh[1];

    to_list = ({ replyh[0] });
    if ( reply_all )
	to_list = clean_array(to_list + replyh[2] + replyh[3]);

    file = tmp_fname();
    write_file(file, body);
    clone_object(EDIT_OB)->edit_file(file, "mailer_done_edit", 0);
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
    int		key;
    mixed	headers;
    string *	body;

    key = get_message_key(user_num);
    if ( !key ) return;

    headers = MAIL_D->get_headers( ({ key }) )[0];

    body = build_body_inclusion(key);
    body = ({"Begin forwarded message:",
		 sprintf("On %s %s wrote:",
			 headers[4],
			 capitalize(headers[0]))
	 }) + body;

    (void)send_mail_message("FWD: " + headers[1],
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
