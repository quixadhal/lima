/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** oob.c -- code for handling I3's OOB services
**
** 960125, Deathblade: created
*/

#include <log.h>
#include <socket.h>
#include <ports.h>

mixed unguarded(mixed priv, function func);

void send_to_mud(string type, string mudname, mixed * message);
void log_error_rcv(string mudname, mixed * message);
void log_error_snd(string mudname, mixed * message);

void do_auth_mud_req(string mudname);
int validate_auth(string mudname, int provided_key);

string canon_mudname(string mudname);
mapping query_mudlist();

//void file_process_packet(object socket, mixed * message);
void file_has_outgoing(string remote_mudname);
void file_send_outgoing(string remote_mudname, object socket);
//void mail_process_packet(object socket, mixed * message);
void mail_has_outgoing(string remote_mudname);
void mail_send_outgoing(string remote_mudname, object socket);

/*
** This class is used to record the state of each OOB session.  It is
** used in the oob_socket_map mapping, keyed by the connection's socket
** object.  The same class object is referenced in the values of the
** oob_mudname_map mapping, keyed by the remote mudname.
**
** NOTE: while we are awaiting authentication to open a connection,
** an entry may exist in the oob_mudname_map, but not the corresponding
** oob_socket_map.
*/
class oob_info
{
    object	socket;		/* the socket object */
    string	remote_mudname;	/* the mud we're connected to */

    int		we_originated;	/* did we originate the connection? */
    string	state;		/* current connection state */
    int		activity_time;	/* time of last message */

    string	addr;		/* address of remote OOB port (for opening) */
    function	fail_func;	/* Function to call if we couldn't open a					   connection. */
}

/* map socket objects to oob connection information. */
static private mapping	oob_socket_map = ([ ]);

/* map remote (canonical) mudnames to oob connection information. */
static private mapping	oob_mudname_map = ([ ]);

/*
** General OOB connection states: originator states and target states
*/
#define OOB_STATE_SEND_REQ	"[o] sent oob-req; pausing for now"
#define OOB_STATE_SENT_AUTH	"[o] sent auth-mud-req; awaiting auth-mud-reply"
#define OOB_STATE_SENT_BEGIN	"[o] sent oob-begin; awaiting oob-begin"
#define OOB_STATE_SENT_DATA	"[o] sent request; awaiting reply"
#define OOB_STATE_SENT_END	"[o] sent oob-end; awaiting data or oob-end"
#define OOB_STATE_CLOSE_PENDING	"[o] got oob-end; holding open"

#define OOB_STATE_WAIT_BEGIN	"[t] opened; awaiting oob-begin"
#define OOB_STATE_WAIT_END	"[t] sent oob-begin; awaiting data or oob-end"
#define OOB_STATE_WAIT_REPLY	"[t] sent request; awaiting reply"
#define OOB_STATE_WAIT_CLOSE	"[t] sent oob-end; awaiting data or close"

/* request and reply packets and their corresponding handlers */
static private mapping oob_requests = ([ ]);
static private mapping oob_replies = ([ ]);

/* how long to wait between oob-req and connection */
#define OOB_OPEN_DELAY	2
#define OOB_CLOSE_DELAY	(2*60)		/* keep it open in case of reuse */

/* authentication types */
#define OOB_AUTH_TYPE_NONE	0	/* none used */
#define OOB_AUTH_TYPE_MUD	1	/* auth-mud-req used */

/* torch connection info after this amount of inactivity */
#define OOB_INACTIVITY_TIMEOUT	(10*60)	/* 10 minute timeout */
#define OOB_CLEANUP_TIME	150	/* how often for timeout/cleanup */

void oob_cleanup();
static private function oob_cleanup_func = (: oob_cleanup :);
static private int oob_cleanup_running;

//### driver can't remove a func ptr callout. need a string
#define oob_cleanup_func "oob_cleanup"

/* the OOB listening socket */
static private object	oob_socket;


private nomask void oob_close(class oob_info info)
{
    if ( info->socket )
    {
	map_delete(oob_socket_map, info->socket);
	catch(info->socket->remove());
    }
    map_delete(oob_mudname_map, info->remote_mudname);
}

private nomask int oob_send(class oob_info info, mixed * message)
{
DBBUG(message);
    if ( catch(info->socket->send(message)) )
    {
	oob_close(info);
	return 1;
    }
    return 0;
}
private nomask void oob_error(class oob_info info,
			      string errcode, string errmsg,
			      mixed * errpacket)
{
    mixed * message = ({ errcode, errmsg, errpacket });

    oob_send(info, ({ "error", 5,
			  mud_name(), 0,
			  info->remote_mudname, 0
			  }) + message);
    log_error_snd(info->remote_mudname, message);
}

static nomask void oob_svc_send(object socket, mixed * message)
{
    class oob_info info = oob_socket_map[socket];

    if ( !info )
	error("socket is not an OOB socket\n");

    oob_send(info, message);
}
static nomask void oob_svc_error(object socket,
				 string errcode, string errmsg,
				 mixed * errpacket)
{
    class oob_info info = oob_socket_map[socket];

    if ( !info )
	error("socket is not an OOB socket\n");

    oob_error(info, errcode, errmsg, errpacket);
}

private nomask void oob_cleanup_map(mapping conn_map)
{
    foreach ( class oob_info info in values(conn_map) )
	if ( info->activity_time + OOB_INACTIVITY_TIMEOUT < time() ||
	     ( info->state == OOB_STATE_CLOSE_PENDING &&
	       info->activity_time  + OOB_CLOSE_DELAY < time() ) )
	{
	    oob_close(info);
	}
}

/* cleanup timed-out connections */
private nomask void oob_cleanup()
{
    oob_cleanup_map(oob_mudname_map);
    oob_cleanup_map(oob_socket_map);

    if ( sizeof(oob_mudname_map) + sizeof(oob_socket_map) )
	call_out(oob_cleanup_func, OOB_CLEANUP_TIME);
    else
	oob_cleanup_running = 0;
}

/* do we have outgoing information queued up for this mud? */
private nomask int oob_has_outgoing(string remote_mudname)
{
    return ( mail_has_outgoing(remote_mudname) ||
	     file_has_outgoing(remote_mudname) );
}

private nomask void oob_send_outgoing(class oob_info info)
{
    if ( mail_send_outgoing(info->remote_mudname, info->socket) )
	return;
    file_send_outgoing(info->remote_mudname, info->socket);
}

private nomask void oob_callback_read(object socket, mixed * message)
{
    class oob_info info = oob_socket_map[socket];
    function f_request;
    function f_reply;

DBBUG(message);

    if ( !message )
    {
	info = new(class oob_info);
	info->socket = socket;
	info->remote_mudname = "(not yet provided)";
	info->state = OOB_STATE_WAIT_BEGIN;
	info->activity_time = time();

	oob_socket_map[socket] = info;

	if ( !oob_cleanup_running )
	{
	    call_out(oob_cleanup_func, OOB_CLEANUP_TIME);
	    oob_cleanup_running = 1;
	}

	return;
    }

    /* remember that we heard something from the other guy */
    info->activity_time = time();

    f_request = oob_requests[message[0]];
    f_reply = oob_replies[message[0]];

    /*
    ** If a request just came in and we're waiting for data or a close,
    ** then flip states to where we are expecting this data or an oob-end
    ** packet.  e.g. remaining in this state would barf when an oob-end
    ** arrived.
    **    */
//### it would be nice in the I3 spec to have this transition be
//### explicit...
    if ( f_request && info->state == OOB_STATE_WAIT_CLOSE )
	info->state = OOB_STATE_WAIT_END;

    /*
    ** If a request has come in and we're in the wrong state for it,
    ** then throw back an error and ignore the packet.
    **
    ** Note: we must ignore the packet. If we aren't in one of these
    ** two states, then we have not authenticated the remote mud.  It
    ** would be bad to allow that mud to deliver stuff to us :-)
    */
    if ( f_request &&
	 info->state != OOB_STATE_SENT_END &&
	 info->state != OOB_STATE_WAIT_END )
    {
	oob_error(info, "bad-proto", "packet came at wrong time", message);
	return;
    }

    switch ( message[0] )
    {
//### it would be nice to have different packets for the originator vs.
//### the target mud
    case "oob-begin":
	if ( info->state == OOB_STATE_SENT_BEGIN )
	{
	    /* this is a reply to our oob-begin; trigger outgoing data */
	    f_reply = (function)1;
	}
	else if ( info->state == OOB_STATE_WAIT_BEGIN )
	{
	    /* somebody connected to us. validate their tokens. */
	    if ( !validate_auth(message[1], message[3]) )
	    {
		LOG_D->log(LOG_I3_ERROR,
			   sprintf("%s failed the authentication test\n",
				   message[1]));
		oob_close(info);
	    }
	    else
	    {
		info->remote_mudname = message[1];
		if ( oob_mudname_map[info->remote_mudname] )
		{
#if 0   // Consistancy problems here... -- Rust
		    oob_error(info,
			      "bad-connection",
			      "you are already connected",
			      message);
		    oob_close(info);
		    return;
#endif
		    oob_close(oob_mudname_map[info->remote_mudname]);
		}

		oob_mudname_map[info->remote_mudname] = info;

		oob_send(info, ({ "oob-begin", mud_name(), 0, 0 }));
		info->state = OOB_STATE_WAIT_END;
	    }
	}
	else
	{
	    oob_error(info, "bad-proto", "unexpected oob-begin", message);
	}
	break;

//### it would be nice to have different packets for the originator vs.
//### the target mud
    case "oob-end":
	if ( info->state == OOB_STATE_SENT_END )
	{
	    /*
	    ** We're the originator. If the target is done, then we can
	    ** send more packets or close the connection.
	    */
	    if ( oob_has_outgoing(info->remote_mudname) )
	    {
		oob_send_outgoing(info);
		info->state = OOB_STATE_SENT_DATA;
	    }
	    else
	    {

#if 0		
	        // You can't do this, because then the other side can't
		// init a connection -- Rust
	        /*
		** Wait to close the connection.  We may "reopen" the
		** connection during this time period.  Note that the
		** period cleanup routine will close this as necessary.
		*/
		info->state = OOB_STATE_CLOSE_PENDING;
#else
	   oob_close(info);
#endif
	    }
	}
	else if ( info->state == OOB_STATE_WAIT_END )
	{
	    /*
	    ** We're the target.  When the originator is done, then we
	    ** can send some packets or indicate we have nothing.
	    */
	    if ( oob_has_outgoing(info->remote_mudname) )
	    {
		oob_send_outgoing(info);
		info->state = OOB_STATE_WAIT_REPLY;
	    }
	    else
	    {
		oob_send(info, ({ "oob-end", mud_name() }));
		info->state = OOB_STATE_WAIT_CLOSE;
	    }
	}
	else
	{
	    oob_error(info, "bad-proto", "unexpected oob-end", message);
	}
	break;

    case "error":
      BBUG(message);
	log_error_rcv(info->remote_mudname, message[6..]);
	break;

    case "oob-error":
	log_error_rcv(info->remote_mudname, message[1..]);
	break;

    default:
	if ( f_request )
	    evaluate(f_request, info->remote_mudname, socket, message);
	else if ( f_reply )
	    evaluate(f_reply, info->remote_mudname, socket, message);
	else
	    oob_error(info, "unk-type", "unknown packet type", message);
	break;
    }

    /*
    ** If we just finished processing a reply to one of our requests,
    ** then send more data or indicate we are done and transition to
    ** a new state (appropriate to whether we opened the connection
    ** or were connected to).  Note that we should be in the SENT_DATA
    ** or the WAIT_REPLY state.  We can remain in that state.
    */
    if ( f_reply )
    {
	if ( oob_has_outgoing(info->remote_mudname) )
	{
	    oob_send_outgoing(info);
	}
	else
	{
	    oob_send(info, ({ "oob-end", mud_name() }));
	    if ( info->we_originated )
		info->state = OOB_STATE_SENT_END;
	    else
		info->state = OOB_STATE_WAIT_CLOSE;
	}
    }
}

private nomask void oob_callback_close(object socket)
{
    class oob_info info = oob_socket_map[socket];

    if ( info )
    {
	map_delete(oob_socket_map, info->socket);
	map_delete(oob_mudname_map, info->remote_mudname);
    }
}

private nomask void oob_open_connection(string mudname,
					int auth_type, int auth_token)
{
    class oob_info info = oob_mudname_map[mudname];
    object skt;

DBBUG("opening connection");
    if ( catch(skt =
	         unguarded(1, (: clone_object, SOCKET, SKT_STYLE_CONNECT_M,
			       info->addr,
			       (: oob_callback_read :),
			       (: oob_callback_close :) :) )) )
    {
	oob_close(info);
	return;
    }
    info->socket = skt;
    oob_socket_map[skt] = info;

    if ( oob_send(info,
		  ({ "oob-begin", mud_name(), auth_type, auth_token })) )
	return;

    info->state = OOB_STATE_SENT_BEGIN;
}

varargs nomask void oob_initiate_connection(string mudname, function fail_func)
{
    class oob_info info;
    mixed * mudinfo;

    mudname = canon_mudname(mudname);
    if ( !mudname )
	error("unknown mud\n");
    if ( !oob_has_outgoing(mudname) )
    {
	/* nothing to send to the other mud, so ignore the initiate */
	return;
    }
    if ( info = oob_mudname_map[mudname] )
    {
	/*
	** Already connected.  See if we are in CLOSE_PENDING.  If so,
	** then send an outgoing request and move back to SENT_DATA.
	*/
	if ( info->state == OOB_STATE_CLOSE_PENDING)
	{
	    oob_send_outgoing(info);
	    info->state = OOB_STATE_SENT_DATA;
	}

	return;
    }

    info = new(class oob_info);
    info->remote_mudname = mudname;
    info->we_originated  = 1;
    info->activity_time  = time();
    info->fail_func = fail_func;

    oob_mudname_map[mudname] = info;

    if ( !oob_cleanup_running )
    {
	call_out(oob_cleanup_func, OOB_CLEANUP_TIME);
	oob_cleanup_running = 1;
    }

    mudinfo = query_mudlist()[mudname];
    info->addr = mudinfo[1] + " " + mudinfo[3];

    if ( mudinfo[11]["auth"] )
    {
	/* they have the auth service.  send a request for a key. */
	do_auth_mud_req(mudname);

	info->state = OOB_STATE_SENT_AUTH;
    }
    else
    {
	/* send an oob-req to get them to open a port and then connect */
	send_to_mud("oob-req", mudname, ({ }));

	call_out((: oob_open_connection, mudname, OOB_AUTH_TYPE_NONE, 0 :),
		 OOB_OPEN_DELAY);
    }
}

static nomask void oob_handle_auth_mud_reply(string mudname, int session_key)
{
    class oob_info info = oob_mudname_map[mudname];

    if ( !info || info->state != OOB_STATE_SENT_AUTH )
	return;

    oob_open_connection(mudname, OOB_AUTH_TYPE_MUD, session_key);
}

static nomask void oob_register_requests(mapping requests)
{
    oob_requests += requests;
}
static nomask void oob_register_replies(mapping replies)
{
    oob_replies += replies;
}

nomask void oob_debug_close()
{
    foreach ( string mudname, class oob_info info in oob_mudname_map )
	oob_close(info);
    foreach ( object socket, class oob_info info in oob_socket_map )
	oob_close(info);

    if ( oob_cleanup_running )
    {
	remove_call_out(oob_cleanup_func);
	oob_cleanup_running = 0;
    }
}

static string stat_me()
{
    class oob_info *info_list;
    string result;

    result = "\nOOB TCP SOCKET: ";
    if ( oob_socket )
	result += oob_socket->stat_me();
    else
	result += "<none>\n";

    info_list = clean_array(values(oob_socket_map) + values(oob_mudname_map));

    foreach ( class oob_info info in info_list)
    {
	int idle = time() - info->activity_time;

	if ( !info->remote_mudname )
	    result += sprintf("Unknown incoming connection.  Idle %d seconds.\n", idle);
	else if ( info->we_originated )
	    result += sprintf("-> %s: %s.  Idle %d seconds.\n",
			      info->remote_mudname, info->state, idle);
	else
	    result += sprintf("<- %s: %s.  Idle %d seconds.\n",
			      info->remote_mudname, info->state, idle);
    }

    result += sprintf("OOB cleanup is%s running.\n", oob_cleanup_running ? "" : " not");

    return result;
}

static nomask void oob_shutdown()
{
    foreach ( object socket in keys(oob_socket_map) )
	if ( objectp(socket) )
	    catch(socket->remove());

    if ( oob_socket )
	oob_socket->remove();
}

static nomask void oob_startup()
{
    string err;

    err = catch(oob_socket = new(SOCKET, SKT_STYLE_LISTEN_M,
				 PORT_I3_TCP_OOB,
				 (: oob_callback_read :),
				 (: oob_callback_close :)));
    if ( err )
    {
	oob_socket->remove();
	oob_socket = 0;
	error(err);
    }
}
