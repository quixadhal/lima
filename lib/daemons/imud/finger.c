/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** finger.c -- intermud finger
**
** 95-May-15.  Deathblade.  Created.
*/

#include <daemons.h>

void send_to_mud(string type, string mudname, mixed * message);
void send_to_user(string type, string mudname, string username,
		  mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
string canon_mudname(string mudname);

nomask void do_finger(string mudname, string username)
{
    send_to_mud("finger-req", canon_mudname(mudname), ({ lower_case(username) }));
}

static nomask void rcv_finger_req(string orig_mud, string orig_user,
				  string targ_user, mixed * message)
{
    object p;
    string * info;

    info = FINGER_D->get_raw_data(message[0]);

    send_to_user("finger-reply", orig_mud, orig_user, info);
}

static nomask void rcv_finger_reply(string orig_mud, string orig_user,
				    string targ_user, mixed * message)
{
    object p;

    p = find_body(targ_user);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("Returned finger-reply to unknown user '%s'",
			     targ_user));
    }
    else
    {
	string s;

	if ( message[2] )
	    s = sprintf("Name:  %-35sIn real life: %s\n",
			message[0], message[2]);
	else
	    s = sprintf("Name:  %s\n", message[0]);
	if ( message[3] )
	    s += sprintf("%' '42sEmail: %s\n", "", message[3]);
	s += sprintf("Level: %-35sTitle: %s\n",
		     message[7] ? message[7] : "<unknown>",
		     message[1] ? message[1] : "<unknown>");
	if ( message[4] == -1 )
	    s += "Not logged on.\n";
	else if ( message[4] != "" && message[6] )
	    s += sprintf("On since %s from %s\n", message[4], message[6]);
	else if ( message[4] != "" )
	    s += sprintf("On since %s\n", "<some time>", message[6]);
	if ( message[8] )
	    s += message[8];

	tell_object(p, s);
    }
}
