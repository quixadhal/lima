/* Do not remove the headers from this file! see /USAGE for more info. */

/*/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** emoteto.c -- intermud emoteto
**
** 95-Dec-15.  Winddle.     Created. (From Tell by Deathblade)
*/

#include <commands.h>

void send_to_user(string type, string mudname, string username,
		  mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
string canon_mudname(string mudname);

nomask void do_emoteto(string mudname, string username, string message)
{
  if(message[<1] == '\n')
    message = message[0..<2];
  
  send_to_user("emoteto", canon_mudname(mudname), lower_case(username),
	       ({ this_body()->query_name(), message }));
}

protected nomask void rcv_emoteto(string orig_mud, string orig_user,
			    string targ_user, mixed * message)
{
    object p;
    p = find_body(targ_user);
    message[1]=replace_string(message[1],"$N",sprintf("%s@%s",capitalize(orig_user),capitalize(orig_mud)));
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("'%s' is unknown", targ_user));
    }
    else
    {
	string user = sprintf("%s@%s", message[0], orig_mud);
	tell(p, sprintf("%%^TELL%%^(tell from %s)%%^RESET%%^ %s\n",
			user,
			replace_string(message[1],"$N",sprintf("%s@%s",capitalize(orig_user),capitalize(orig_mud)))));
						       
        CMD_OB_TELL->add_history(p->query_userid(),
                                 sprintf("%%^TELL%%^(tell from %s)%%^RESET%%^ %s\n",
					 user,
					 replace_string(message[1],"$N",sprintf("%s@%s",capitalize(orig_user),capitalize(orig_mud)))));
	p->set_reply(message[0] + "@" + orig_mud);
    }
}
