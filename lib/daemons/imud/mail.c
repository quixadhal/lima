/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mail.c -- handler for I3's "mail" service
**
** 950126, Deathblade: created
*/

void oob_register_requests(mapping requests);
void oob_register_replies(mapping replies);
void oob_initiate_connection(string target_mudname);
void oob_svc_send(object socket, mixed * message);
private nomask void mail_add_request(string, mixed array);

static private mapping mail_requests = ([]);

private nomask void handle_mail(string mudname,
				object socket,
				mixed * message)
{
  string array 	errors;

  errors = IMAIL_D->incoming_mail(mudname, message);
  oob_svc_send(socket, ({"mail-ack", ([ message[1] : errors ])}));
}

private nomask void handle_mail_ack(string mudname,
				    object socket,
				    mixed * message)
{
  IMAIL_D->receive_ack(message[1]);
}

static nomask int mail_has_outgoing(string mudname)
{
    return sizeof(mail_requests[mudname]) != 0;
}

static nomask int mail_send_outgoing(string mudname, object socket)
{
    mixed array	requests = mail_requests[mudname];

    if (!sizeof(requests))
    {
	return 0;
    }

    oob_svc_send(socket, requests[0]);
    mail_requests[mudname] = requests[1..];
}

static nomask void mail_startup()
{
    oob_register_requests(([ "mail" : (: handle_mail :),
				 ]));
    oob_register_replies(([ "mail-ack" : (: handle_mail_ack :),
				]));
}

private nomask void mail_add_request(string mudname, mixed array request)
{
    if ( !mail_requests[mudname] )
      {
	mail_requests[mudname] = request;
      }
    else
      {
	mail_requests[mudname] += request;
      }
}

public nomask void send_mail_message_to_mud(array packet, string mudname)
{
  if(previous_object() != find_object(IMAIL_D))
    {
      error("Fuck no");
    }

  mail_add_request(mudname, ({packet}));
  oob_initiate_connection(mudname);
}
