/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** emoteto.c -- intermud emoteto
**
** 95-Dec-15.  Winddle.     Created. (From Tell by Deathblade)
*/

void send_to_user(string type, string mudname, string username,
		  mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
string canon_mudname(string mudname);

nomask void do_emoteto(string mudname, string username, string message)
{
    if(strsrch(message, "$N") == -1)
	message = "$N " + message;
    
    if(message[<1] == '\n')
        message = message[0..<2];

    send_to_user("emoteto", canon_mudname(mudname), lower_case(username),
		 ({ this_body()->query_name(), message }));
}

static nomask void rcv_emoteto(string orig_mud, string orig_user,
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
	string user = sprintf("%s@%s", message[0], orig_mud);
	tell_object(p, sprintf("*%s\n",
			       replace_string(message[1], "$N", user)));
						       
	p->set_reply(message[0] + "@" + orig_mud);
    }
}
