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

#include <mudlib.h>
#include <socket.h>
#include <security.h>
#include <log.h>

inherit M_ACCESS;
inherit M_RECONNECT;

inherit "/daemons/imud/tell";
inherit "/daemons/imud/emoteto";
inherit "/daemons/imud/who";
inherit "/daemons/imud/finger";
inherit "/daemons/imud/locate";
inherit "/daemons/imud/channel";
inherit "/daemons/imud/mudlist";

#define OOB_PORT	(__PORT__ + 1)
#define SAVE_FILE	"/data/daemons/imud_d"

static private object	router_socket;
static private object	oob_socket;

private string *	router_list = ({ ({ "*gjs", "198.174.169.120 9000"}) });
private int		password;

void rcv_startup_reply(string orig_mud, string orig_user,
		       string targ_user, mixed * message);
void rcv_error(string orig_mud, string orig_user,
	       string targ_user, mixed * message);


static private mapping	dispatch =
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
  "channel-t" : (: 0 :),
  "chan-user-req" : (: 0 :),
  "chan-user-reply" : (: 0 :),

  "startup-reply" : (: rcv_startup_reply :),
  "error" : (: rcv_error :),
  "mudlist" : (: rcv_mudlist :),
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

static void send_to_router(string type, mixed * message)
{
    send_message(type, "*gjs", 0, message);
}

static void send_to_mud(string type, string mudname, mixed * message)
{
    send_message(type, mudname, 0, message);
}

static void send_to_user(string type, string mudname, string username,
			 mixed * message)
{
    send_message(type, mudname, username, message);
}

static void send_to_all(string type, mixed * message)
{
    send_message(type, 0, 0, message);
}

static void return_error(string mudname, string username,
			 string errcode, string errmsg)
{
    send_message("error", mudname, username, ({ errcode, errmsg, 0 }));
}

/* handle reads from the router */
private nomask void handle_router_read(object socket, mixed * message)
{
if (message[0]!= "mudlist")
    DBBUG(message);

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

    err = catch(router_socket = new(SOCKET, SKT_STYLE_CONNECT_M,
				    router_list[0][1],
				    (: handle_router_read :),
				    (: handle_router_close :)));

    if ( err )
    {
	DBBUG(err);

	router_socket->remove();
	router_socket = 0;

	trigger_reconnect("router");
    }
    else
    {
	send_to_router("startup-req-3",
		       ({ password,
			      query_mudlist_id(),
			      query_chanlist_id(),
			      __PORT__,
			      OOB_PORT,
			      0,
/* DO NOT change this; see comments in /secure/user/login.c */
			      "Lima 0.9r10 (pre-alpha)",
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
				  ]),
			      0,	/* other_data */
			      }));
    }
}

/* handle reads on the OOB socket (or an accepted socket) */
private nomask void handle_oob_read(object socket, mixed * message)
{
}

/* handle closures of the OOB socket (or an accepted socket) */
private nomask void handle_oob_close(object socket)
{
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

    set_privilege(1);

    restore_object(SAVE_FILE);
    mudlist_reset_entries();

    reconn_func = (: reconnect :);

    err = catch(oob_socket = new(SOCKET, SKT_STYLE_LISTEN_M,
				 OOB_PORT,
				 (: handle_oob_read :),
				 (: handle_oob_close :)));
    if ( err )
    {
	DBBUG(err);

	oob_socket->remove();
	oob_socket = 0;
	error(err);
    }

    trigger_reconnect("router");

    init_channels();
}

void remove(int coming_back_soon)
{
    /*
    ** coming_back_soon is 0 in most cases, 1 for an update.  This is just
    ** the right value to tell the router when we might be back online.
    */
    send_to_router("shutdown", ({ coming_back_soon }));

    unguarded(1, (: save_object, SAVE_FILE :));

    if ( oob_socket )
	oob_socket->remove();
    if ( router_socket )
	router_socket->remove();

    free_channels();
    destruct();
}


private nomask void rcv_startup_reply(string orig_mud, string orig_user,
				      string targ_user, mixed * message)
{
    if ( message[0][0][0] != router_list[0][0] )
    {
//### need to disconnect and reconnect to new router
    }

    router_list = message[0];
    password = message[1];

    /* reset the reconnection timers so the next reconnect is 1 minute */
    cancel_reconnection("router");
}

private nomask void rcv_error(string orig_mud, string orig_user,
			      string targ_user, mixed * message)
{
    object ob;

    if ( targ_user && (ob = find_user(targ_user)) )
    {
	tell_object(ob, sprintf("Intermud tells you: %s: %s\n",
				message[0], message[1]));
    }
    else
    {
	LOG_D->log(LOG_I3_ERROR,
		   sprintf("%s: %s\n%O\n", message[0], message[1],
			   message[2]));

	NCHANNEL_D->deliver_channel("wiz_errors",
				    sprintf("I3 (%s): %s\n",
					    message[0],
					    message[1]));
    }
}

int stat_me()
{
    write("\nROUTER SOCKET: ");
    if ( router_socket )
	router_socket->stat_me();
    else
	write("<none>\n");

    write("\nOOB TCP SOCKET: ");
    if ( oob_socket )
	oob_socket->stat_me();
    else
	write("<none>\n");

    reconnect::stat_me();

    return 1;
}
