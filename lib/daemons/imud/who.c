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

protected nomask void rcv_who_req(string orig_mud, string orig_user,
			       string targ_user, mixed * message)
{
    mixed * who_data;

    who_data = map_array(users(),
			 function (object user)
			 {
			     object body = user->query_body();
			     string visname = user->query_userid();
			     string title = 0;

			     if ( body )
			     {
                                 if ( !body->is_visible() ) return 0;
				 visname = body->query_name();
				 title = body->query_title();
			     }
			     else if ( !(visname = user->query_userid()) )
				 return 0;
			     else
				 capitalize(visname);

			     return ({ visname,
					   query_idle(user),
					   title });
			 } ) - ({ 0 });

    send_to_user("who-reply", orig_mud, orig_user, ({ who_data }));
}

protected nomask void rcv_who_reply(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
    object p;

    p = find_user(targ_user);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("Returned who-reply to unknown user '%s'",
			     targ_user));
    }
    else
    {
	string s;

        s = sprintf("[%s] %d users connected\n%s\n", orig_mud, sizeof(message[0]), repeat_string("-", 78));
	s += implode(map_array(message[0],
			       (: sprintf("%-15s%-10s%s",
					  $1[0], convert_time(to_int($1[1]), 3), $1[2]) :)), "\n");
	s += "\n" + repeat_string("-", 78) + "\n";
	tell(p, s);
    }
}
