/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** locate.c -- intermud user locating
**
** 95-May-15.  Deathblade.  Created.
*/

void send_to_all(string type, mixed * message);
void send_to_user(string type, string mudname, string username,
		  mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);

nomask void do_locate(string username)
{
    send_to_all("locate-req", ({ lower_case(username) }));
}

static nomask void rcv_locate_req(string orig_mud, string orig_user,
				  string targ_user, mixed * message)
{
    object p;

    p = find_body(message[0]);
    if ( p )
    {
	send_to_user("locate-reply", orig_mud, orig_user,
		     ({ mud_name(), p->query_name() }));
    }
}

static nomask void rcv_locate_reply(string orig_mud, string orig_user,
				    string targ_user, mixed * message)
{
    object p;

    p = find_body(targ_user);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("Returned locate-reply to unknown user '%s'",
			     targ_user));
    }
    else
    {
	tell_object(p, sprintf("[locate] %s has been found on %s.\n",
			       message[1], message[0]));
    }
}
