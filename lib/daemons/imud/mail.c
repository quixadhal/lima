/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mail.c -- handler for I3's "mail" service
**
** 950126, Deathblade: created
*/

void oob_register_requests(mapping requests);
void oob_register_replies(mapping replies);


private nomask void handle_mail(string mudname,
				object socket,
				mixed * message)
{
}

private nomask void handle_mail_ack(string mudname,
				    object socket,
				    mixed * message)
{
}

static nomask void mail_process_packet(mixed * message)
{
    switch ( message[0] )
    {
    case "mail":
	break;

    case "mail-ack":
	break;
    }
}

static nomask int mail_has_outgoing(string remote_mudname)
{
    return 0;
}

static nomask int mail_send_outgoing(string remote_mudname, object socket)
{
    return 0;
}

static nomask void mail_startup()
{
    oob_register_requests(([ "mail" : (: handle_mail :),
				 ]));
    oob_register_replies(([ "mail-ack" : (: handle_mail_ack :),
				]));
}
