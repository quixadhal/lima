/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Mh.c
**
** Command-line driven mailer.
**
** Written by Rust at some point in time.
** SEP-NOV '94: Updated/revised by Deathblade (for all kinds of stuff).
*/

#include <playerflags.h>
#include <mudlib.h>
#include <daemons.h>

inherit MAILER;


private void Mh_cmd_write(string arg)
{
    int	num;
    string fname;

    if(!arg || (sscanf(arg,"%d %s",num,fname)!=2&&
	(sscanf(arg,"%s",fname)!=1)))
    {
	write("Usage: Mw # <filename>\n");
	return;
    }

    cmd_read(num, fname, 1);
}


private void Mh_cmd_save(string arg)
{
    int	num;
    string fname;

    if(!arg || (sscanf(arg,"%d %s",num,fname)!=2&&
	sscanf(arg,"%s",fname)!=1))
    {
	write("Usage: Ms # <filename>\n");
	return;
    }

    cmd_read(num, fname, 0);
}


private void Mh_cmd_replyall(mixed arg)
{
    if ( arg && !(arg = to_int(arg)) )
    {
	write("Usage: MR #\n");
	return;
    }

    cmd_reply(arg, 1);
}


private void Mh_cmd_reply(mixed arg)
{
    if ( arg && !(arg = to_int(arg)) )
    {
	write("Usage: Mr #\n");
	return;
    }

    cmd_reply(arg, 0);
}


private void Mh_cmd_help()
{
    string* output;

    output = explode(
      "Help for Mh (Mail handler)  Version 0.9\n"+sprintf("%'-'40s","\n")+
      "M?					This help screen\n"
      "\n"
      "Mm <name1> [name2] [name3]...		Send mail.\n"
      "Mh [# or range (1-4,6 for example)]	Show the headers for the specified\n"
      "					range of messages, or all messages\n"
      "					if no range is specified.\n"
      "M# # 					Read a message.\n"
      "Mr #					Reply to message\n"
      "MR #					Reply to message, all\n"
      "					originial recipients receive the mail\n"
      "Mf #					Forward message\n"
      "Md # or range				Delete message(s)\n"
      "M$					Show the number of messages in your box.\n"
      "M= [#]					Set current message if # is provided\n"
      "					else shows the current message #\n"
      "M+					increment the current message number\n"
      "					but do not read.\n"
      "M-					decrement the current message number\n"
      "					but do not read.\n"
      "Mn					read the next message."
      ,"\n");
    if( wizardp(this_user()) )
	output += ({
	  "Ms [#] <filename>			Save message w/ header intact to file.",
	  "Mw [#] <filename>			Save message but no header to file." });
    output+=({"",
      "All numbers default to the currently referenced message, which is shown",
      "by M=\n",
      "","Groups may also be mailed to, see help for groups on how to set them",
      "up.... you can mail to them like a regular name, or you can enclose"
      ,"The group in ().  Eg, Mm (admin) rust  will mail all the admins and rust."});
    clone_object(MORE_OB)->more_string(output);
}


private void Mh_cmd_forward(mixed arg)
{
    mixed	newto;
    int		num;

    if(!arg || (sscanf(arg,"%d %s",num,newto)!=2&&
		(sscanf(arg,"%s",newto)!=1)))
    {
	write("Usage: Mf # <name1> [name2] ...\n");
	return;
    }

    cmd_forward(num, newto);
}


nomask int parse_Mh_cmd(string input)
{
    string	arg;

    /* ### we need to validate the caller! */


    if( strlen(input) > 2 && input[1] == ' ' )
	arg = input[2..];
    else if( strlen(input) > 2 )
	return 0;

    switch( input[0] )
    {
    case 'h':
	cmd_headers( arg );
	return 1;
    case 'm':
	cmd_mail( arg );
	return 1;
    case 'R':
	Mh_cmd_replyall( arg );
	return 1;
    case 'r':
	Mh_cmd_reply( arg );
	return 1;
    case '?':
	Mh_cmd_help();
	return 1;
    case 'd': 
	cmd_delete( arg );
	return 1;
    case 'f':
	Mh_cmd_forward( arg );
	return 1;
    case '#':
	cmd_read(to_int(arg), 0, 0);
	return 1;
    case 's':
	Mh_cmd_save( arg );
	return 1;
    case '=':
	cmd_setcurrent( arg );
	return 1;
    case 'w':
	Mh_cmd_write( arg );
	return 1;
    case 'n':
    {
        int idx;

        idx = mailbox_ob->first_unread_message() + 1;
        if ( !idx )
            printf("No more messages.\n");
        else
            cmd_read(idx, 0, 0);
	return 1;
    }
    case '+':
	cmd_setcurrent(mailbox_ob->query_message_index()+2 );
	return 1;
    case '-':
	cmd_setcurrent(mailbox_ob->query_message_index() );
	return 1;
    case '$':
	printf("You have %d messages.\n", mailbox_ob->query_message_count());
	return 1;
    default:
	return 0;
    }
}

/*
** Compat with the modal-version.  Tell user what is up.
*/
nomask void begin_mail(string arg)
{
    write("You are set up for Mh, the command-line mail handler.\n"
	  "Type M? for info.\n");
}
