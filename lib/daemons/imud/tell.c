/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** tell.c -- intermud tell
**
** 95-May-13.  Deathblade.  Created.
*/

void send_to_user(string type, string mudname, string username,
		  mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
string canon_mudname(string mudname);

nomask void do_tell(string mudname, string username, string message)
{
    send_to_user("tell", canon_mudname(mudname), lower_case(username),
		 ({ this_body()->query_name(), message }));
}

static nomask void rcv_tell(string orig_mud, string orig_user,
			    string targ_user, mixed * message)
{
    object p;

    p = find_body(targ_user);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("'%s' is unknown", targ_user));
    }
    else
    {
	tell_object(p, sprintf("%s@%s tells you: %s\n",
			       message[0], orig_mud, message[1]));
	p->set_reply(message[0]+"@" + orig_mud);
    }
}
