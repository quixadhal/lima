/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** tell.c -- intermud tell
**
** 95-May-13.  Deathblade.  Created.
*/

#include <commands.h>

void send_to_user(string type, string mudname, string username,
                  mixed * message);
void return_error(string mudname, string username,
                  string errcode, string errmsg);
string canon_mudname(string mudname);

nomask void do_tell(string mudname, string username, string message)
{
    send_to_user("tell", canon_mudname(mudname), lower_case(username),
                 ({ capitalize(this_user()->query_userid()), message }));
}

protected nomask void rcv_tell(string orig_mud, string orig_user,
                            string targ_user, mixed * message)
{
    object p;

    p = find_user(targ_user);
    if ( !p )
    {
        return_error(orig_mud, orig_user, "unk-user",
                     sprintf("'%s' is unknown", targ_user));
    }
    else
    {
        tell(p, sprintf("%%^TELL%%^%s@%s tells you:%%^RESET%%^ %s\n",
                        message[0], orig_mud, message[1]));
        p->add_tell_history(
                                 sprintf("%%^TELL%%^%s@%s tells you:%%^RESET%%^ %s\n",
                                         message[0], orig_mud, message[1]));

        p->set_reply(message[0]+"@" + orig_mud);
    }
}
