/* Do not remove the headers from this file! see /USAGE for more info. */

/* SMTP daemon
 * This daemon will send mail to a specified address using the local SMTP
 * agent.
 * 
 * 971107 Written by Tigran
 */

/* 
 * INCLUDES
 */
#include <socket.h>

/* 
 * INHERIT
 */
inherit M_ACCESS;

/* 
 * FUNCTIONS
 */

/* Shoudn't need to read anything */
private nomask void mail_socket_read(){}
private nomask void mail_socket_close(object socket){}

/* This may need a bit more work in the header.  Still reading the rfc on
 * this one -- tigran */
nomask void send_mail(string destination,string subject,string message)
{
  string reply_to;
  string foo;
  object socket;
  if(!check_privilege("Mudlib:daemons"))
    return;
  reply_to=unguarded(1,
		     (: call_other, 
		        USER_D,
		        "query_variable",
		        this_user()->query_userid(),
		      ({"email"}) :) )[0];
  socket=new(SOCKET,
	     SKT_STYLE_CONNECT,
	     "127.0.0.1 25",
	     (: mail_socket_read :),
	     (: mail_socket_close :) );
  catch 
    {
      socket->send("HELO "__HOST__"\n");
      socket->send("MAIL FROM: <"MUD_NAME" LPMUD>\n");
      socket->send("RCPT TO: " + destination + "\n");
      socket->send("DATA\n");
      socket->send("From: \"" + MUD_NAME + "\" <" + ADMIN_EMAIL + ">\n");
      socket->send("To: <" + destination + ">\n");
      if(reply_to!=""||!reply_to)
	socket->send(sprintf("Reply-To: <%s> (%s@%s)\n",
			     reply_to,
			     capitalize(this_user()->query_userid()),
			     MUD_NAME) );
      socket->send("Subject: " + subject + "\n\n");
      socket->send(message + "\n");
      socket->send(".\n");
      socket->send("QUIT\n");
      //      destruct(socket);
    };
}

private nomask void create()
{
  set_privilege(1);
}
