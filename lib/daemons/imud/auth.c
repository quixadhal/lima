/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** auth.c -- mud authentication packet handling
**
** 960124, Deathblade: created
*/

void send_to_mud(string type, string mudname, mixed * message);
string canon_mudname(string mudname);
void oob_handle_auth_mud_reply(string mudname, int session_key);


#define AUTH_CLEANUP_TIME	(15*60)	/* clean up every 15 minutes */
#define AUTH_EXPIRE_TIME	(10*60)	/* expire after 10 minutes */

/* records information about keys passed to muds */
class auth_data
{
    int session_key;
    int timestamp;
}

/*
** This maps (remote) mudnames to their authentication data.  We do not
** account for the possibility of the daemon being updated between the
** time we issue a session key and the time when a connection attempt is
** made.
*/
nosave private mapping auth_info = ([ ]);

void auth_cleanup();
nosave private function auth_cleanup_func = (: auth_cleanup :);
nosave private int auth_cleanup_running;

//### driver can't remove a func ptr callout. need a string
#define auth_cleanup_func "auth_cleanup"

/* the mudname should be in its canonical form */
protected nomask void do_auth_mud_req(string mudname)
{
    send_to_mud("auth-mud-req", canon_mudname(mudname), ({ }));
}

/* the mudname should be in its canonical form */
protected nomask int validate_auth(string mudname, int provided_key)
{
    class auth_data data = auth_info[mudname];
    int result = data && data->session_key == provided_key;

    map_delete(auth_info, mudname);

    return result;
}

protected nomask void rcv_auth_mud_req(string orig_mud, string orig_user,
				    string target_user, mixed * message)
{
    class auth_data auth = new(class auth_data);

    auth->session_key = random(2000000000);
    auth->timestamp   = time();
    auth_info[orig_mud] = auth;

    send_to_mud("auth-mud-reply", orig_mud, ({ auth->session_key }));

    if ( !auth_cleanup_running )
    {
	call_out(auth_cleanup_func, AUTH_CLEANUP_TIME);
	auth_cleanup_running = 1;
    }
}

protected nomask void rcv_auth_mud_reply(string orig_mud, string orig_user,
				      string target_user, mixed * message)
{
    /* only the OOB system needs this reply */
    oob_handle_auth_mud_reply(orig_mud, message[0]);
}

/*
** This function is called periodically to check for authorization tokens
** that have expired and need to be flushed out.
*/
private nomask void auth_cleanup()
{
    auth_info = filter(auth_info,
		       function (string mudname, class auth_data auth)
		       {
			   return auth->timestamp + AUTH_EXPIRE_TIME > time();
		       } );

    if ( sizeof(auth_info) )
	call_out(auth_cleanup_func, AUTH_CLEANUP_TIME);
    else
	auth_cleanup_running = 0;
}
