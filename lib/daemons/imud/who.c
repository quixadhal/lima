/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** who.c -- intermud who
**
** 95-May-15.  Deathblade.  Created.
*/

void send_to_mud(string type, string mudname, mixed * message);
void send_to_user(string type, string mudname, string username,
		  mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
string canon_mudname(string mudname);

nomask void do_who(string mudname)
{
    send_to_mud("who-req", canon_mudname(mudname), ({ }));
}

static nomask void rcv_who_req(string orig_mud, string orig_user,
			       string targ_user, mixed * message)
{
    mixed * who_data;

    who_data = map_array(users(),
			 (: ({ $1->query_name(),
				   query_idle($1),
				   query_ip_name($1) }) :));
    send_to_user("who-reply", orig_mud, orig_user, ({ who_data }));
}

static nomask void rcv_who_reply(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
    object p;

    p = find_body(targ_user);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("Returned who-reply to unknown user '%s'",
			     targ_user));
    }
    else
    {
	string s;

	s = sprintf("[%s] %d users connected\n%'-'79s\n", orig_mud,
		    sizeof(message[0]), "");
	s += implode(map_array(message[0],
			       (: sprintf("%-15s%-5d%s",
					  capitalize($1[0]),
					  $1[1], $1[2]) :)), "\n");
	s += sprintf("\n%'-'79s\n", "");
	tell_object(p, s);
    }
}
