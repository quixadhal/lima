/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** imud_d.c -- Intermud-3 Daemon
**
** A daemon responsible for processing the Intermud-3 protocol.
**
** The specification for Intermud-3 can be found at:
**   http://www.imaginary.com/intermud/intermud3.html
**
** 95-May-11.  Deathblade.  Created.
*/

#include <socket.h>
#include <security.h>
#include <log.h>
#include <ports.h>

inherit M_DAEMON_DATA;
inherit M_RECONNECT;

inherit __DIR__ "imud/tell";
inherit __DIR__ "imud/emoteto";
inherit __DIR__ "imud/who";
inherit __DIR__ "imud/finger";
inherit __DIR__ "imud/locate";
inherit __DIR__ "imud/channel";
inherit __DIR__ "imud/mudlist";
inherit __DIR__ "imud/auth";
inherit __DIR__ "imud/ucache";
inherit __DIR__ "imud/oob";
inherit __DIR__ "imud/file";
inherit __DIR__ "imud/mail";

nosave private object	router_socket;

private string *	router_list = ({ ({ "*gjs", "208.192.43.105 9000"}) });
private int		password;

void rcv_startup_reply(string orig_mud, string orig_user,
		       string targ_user, mixed * message);
void rcv_error(string orig_mud, string orig_user,
	       string targ_user, mixed * message);
void rcv_oob_req(string orig_mud, string orig_user,
		 string targ_user, mixed * message);


nosave private mapping	dispatch =
([
  "tell" : (: rcv_tell :),
  "emoteto" : (: rcv_emoteto :),

  "who-req" : (: rcv_who_req :),
  "who-reply" : (: rcv_who_reply :),

  "finger-req" : (: rcv_finger_req :),
  "finger-reply" : (: rcv_finger_reply :),

  "locate-req" : (: rcv_locate_req :),
  "locate-reply" : (: rcv_locate_reply :),

  "chanlist-reply" : (: rcv_chanlist_reply :),
  "chan-who-req" : (: rcv_chan_who_req :),
  "chan-who-reply" : (: rcv_chan_who_reply :),
  "channel-m" : (: rcv_channel_m :),
  "channel-e" : (: rcv_channel_e :),
  "channel-t" : (: rcv_channel_t :),
  "chan-user-req" : (: rcv_chan_user_req :),
  "chan-user-reply" : (: rcv_chan_user_reply :),

  "auth-mud-req" : (: rcv_auth_mud_req :),
  "auth-mud-reply" : (: rcv_auth_mud_reply :),

  "ucache-update" : (: rcv_ucache_update :),

  "mudlist" : (: rcv_mudlist :),

  "startup-reply" : (: rcv_startup_reply :),
  "error" : (: rcv_error :),
  "oob-req" : (: rcv_oob_req :),
]);


private nomask void send_message(string type, string target_mud,
				 string target_user, mixed * message)
{
    string orig_user;

    if ( !router_socket )
        return;

    if ( this_user() )
	orig_user = this_user()->query_userid();
    if ( catch(router_socket->send(({ type, 5, mud_name(), orig_user,
					  target_mud,
					  target_user }) + message)))
    {
	write("Error, the router seems to be dead.\n"
	      "Imud stuff isn't going to work till it's back up.\n");
    }
}

protected void send_to_router(string type, mixed * message)
{
    send_message(type, "*gjs", 0, message);
}

protected void send_to_mud(string type, string mudname, mixed * message)
{
    send_message(type, mudname, 0, message);
}

protected void send_to_user(string type, string mudname, string username,
			 mixed * message)
{
    send_message(type, mudname, username, message);
}

protected void send_to_all(string type, mixed * message)
{
    send_message(type, 0, 0, message);
}

protected void return_error(string mudname, string username,
			 string errcode, string errmsg)
{
    send_message("error", mudname, username, ({ errcode, errmsg, 0 }));
}

/* handle reads from the router */
private nomask void handle_router_read(object socket, mixed * message)
{
if (message[0]!= "mudlist")
{
    string s = sprintf("%O", message);
    if ( sizeof(s) > 200 )
	s[200..] = "...";
    DBBUG(s);
}

    if ( !dispatch[message[0]] )
    {
	/* return an error packet */
	send_message("error", message[2], message[3],
		     ({ "unk-type",
			    sprintf("type '%s' is unrecognized", message[0]),
			    message }));
	return;
    }

    evaluate(dispatch[message[0]],
	     message[2], message[3], message[5], message[6..]);
}

/* handle closures of the router socket */
private nomask void handle_router_close(object socket)
{
    DBBUG("router closed");
    router_socket = 0;

    /* mark all the muds as down. when we reconnect we'll get new data */
    mudlist_reset_entries();

//### for now, we need to set the password to 0. the router doesn't send
//### "up" status changes when we reconnect (general problem with the
//### the router -- I'm surprised nobody else has mentioned it)
    password = 0;
       
    trigger_reconnect("router");
}

/* (re)connect to the router */
private nomask void reconnect()
{
    string err;

    err = catch(router_socket =
		  unguarded(1, (: clone_object, SOCKET, SKT_STYLE_CONNECT_M,
				router_list[0][1],
				(: handle_router_read :),
				(: handle_router_close :) :) ));

    if ( err )
    {
	DBBUG(err);

	if ( router_socket )
	{
	    router_socket->remove();
	    router_socket = 0;
	}

	trigger_reconnect("router");
    }
    else
    {
	send_to_router("startup-req-3",
		       ({ password,
			      query_mudlist_id(),
			      query_chanlist_id(),
			      __PORT__,
			      PORT_I3_TCP_OOB,
			      0,
/* DO NOT change this; see comments in /secure/user/login.c */
			      "Lima 1.0a7",
			      "Lima",
			      driver_version(),
			      "LP",
			      lib_status(),
			      ADMIN_EMAIL,
			      ([
				  "tell" : 1,
				  "emoteto" : 1,
				  "who" : 1,
				  "finger" : 1,
				  "locate" : 1,
				  "channel" : 1,
				  "auth" : 1,
				  "ucache" : 1,
				  "file" : 1,
				  "http" : 80,
				  "ftp" : 21,
                                  "mail" : 1
				  ]),
			      0,	/* other_data */
			      }));
    }
  call_out("relisten_all_channels", 5);
}

void create()
{
    string err;

    if ( clonep() )
    {
	destruct(this_object());
	return;
    }
    if ( ADMIN_EMAIL == "user@host.name" )
    {
        write("ERROR:\n"
	      "  The I3 daemon will not load until you set a proper ADMIN_EMAIL\n"
	      "  value in /include/config.h\n");
	destruct(this_object());
	return;
    }

    ::create();

    mudlist_reset_entries();

    reconn_func = (: reconnect :);

    oob_startup();
    chan_startup();
    file_startup();
    mail_startup();

    trigger_reconnect("router");
}

void remove(int coming_back_soon)
{
    /*
    ** coming_back_soon is 0 in most cases, 1 for an update.  This is just
    ** the right value to tell the router when we might be back online.
    */
    send_to_router("shutdown", ({ coming_back_soon }));

    /* save all current state */
    save_me();

    if ( router_socket )
	router_socket->remove();

    chan_shutdown();
    oob_shutdown();

    remove_call_out();

    destruct();
}

protected nomask void log_error_rcv(string mudname, mixed * message)
{
    LOG_D->log(LOG_I3_ERROR, sprintf("(<- %s) %s: %s\n%O\n", mudname,
				     message[0], message[1], message[2]));

    CHANNEL_D->deliver_channel("errors",
				sprintf("I3 (%s): %s",
					message[0],
					message[1]));
}
protected nomask void log_error_snd(string mudname, mixed * message)
{
    LOG_D->log(LOG_I3_ERROR, sprintf("(-> %s) %s: %s\n%O\n", mudname,
				     message[0], message[1], message[2]));
}

private nomask void rcv_startup_reply(string orig_mud, string orig_user,
				      string targ_user, mixed * message)
{
    /*
    ** If the first router listed in the router list is not the one that
    ** we just connected to, then close the connection and connect to
    ** the specified router.
    */
//### we need a fail-over mechanism to go to a another (second) router
    if ( message[0][0][0] != router_list[0][0] ||
	 message[0][0][1] != router_list[0][1] )
    {
	/* tell it we'll be gone indefinitely */
	send_to_router("shutdown", ({ 0 }));
	router_socket->remove();
    }

    router_list = message[0];
    password = message[1];

    /* make sure the new router_list is saved */
    save_me();

    /* reset the reconnection timers so the next reconnect is 1 minute */
    cancel_reconnection("router");

    /* if we don't have a socket any more (closed above), then reopen */
    if ( !router_socket )
    {
	trigger_reconnect("router");
    }
}

private nomask void rcv_error(string orig_mud, string orig_user,
			      string targ_user, mixed * message)
{
    object ob;

    if ( targ_user && (ob = find_user(targ_user)) )
    {
	tell(ob, sprintf("Intermud tells you: %s: %s\n",
			 message[0], message[1]));
    }
    else
    {
	log_error_rcv(orig_mud, message);
    }
}

private nomask void rcv_oob_req(string orig_mud, string orig_user,
				string targ_user, mixed * message)
{
    /*
    ** We always keep the port open and people should be using the
    ** auth service.  There isn't anything to do here right now.
    */
}

string stat_me()
{
    string result = "\nROUTER SOCKET: ";

    if ( router_socket )
	result += router_socket->stat_me();
    else
	result += "<none>\n";

    result +=
	m_reconnect::stat_me() +
	oob::stat_me();

    return result;
}
