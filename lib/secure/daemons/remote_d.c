/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** remote_d.c -- RPC daemon
**
** 13-Apr-95.  Beek.  Created.
** 15-Apr-95.  Deathblade.  Added remote_eval(), added N-way connects,
**	added reconnect processing.
*/
#include <mudlib.h>
#include <socket.h>
#include <security.h>
#include <config.h>
#include <driver/origin.h>

inherit M_ACCESS;
inherit M_RECONNECT;

//:MODULE
//This daemon handles connections to other muds for the purpose of
//remote procedure calls.

#define BEEKBUG(x) TELL_BUG("beek",(x))

/* This could come from the mudlist, but then we are vulnerable to mudlist
 * spoofing.  This is bad, since we implicitly trust these muds.
 *
 * I.E. DO NOT ADD MUDS TO THIS LIST
 */
static private mapping muds = ([ "zork" : "193.10.174.40",
				"quendor" : "193.10.174.40" ]);
static private mapping mud_ports = ([ "zork" : 9999,
				     "quendor" : 9998 ]);

static private mapping dyn_muds = ([ "dbmud" : ({ "156.151.175.", "156.151.176." }) ]);

static private mapping synonyms = ([ "ZorkMUD" : "zork",
				    "zork" : "zork",
				    "Quendor" : "quendor",
				    "quendor" : "quendor",
				    "DB Mud" : "dbmud",
				    "dbmud" : "dbmud",
				    ]);

/*
** Store each of the sockets used for communication.  name -> socket.
** While we are connecting, a socket will reside in connecting[].  Once
** the connection is established (an "echo" is received), then the
** socket will move to sockets[] and communication can begin.
*/
static private mapping sockets = ([ ]);
static private mapping connecting = ([ ]);

/*
** All remote_d objects have a listen socket so that a fully-connected
** network can established between N muds.
*/
static private object our_listen_socket;

/*
** Some remote calls will expect a return value.  This mapping is used
** to hold the outstanding transactions.  The mapping is keyed by the
** transaction id, a monotonically increasing number.
*/
static private mapping	trans = ([ ]);
static private int	transid;

/*
** Authentication tokens.  These are returned to us by remote muds when
** a user "logs in" to the remote mud.  They will be emptied if we lose
** our connection with the mud (it could be bad if reconnected to a rogue
** that replaced a mud).
**
** The variables are:
**	returned_tokens: ([ mudname -> ([ userob -> token ]) ])
**	created_tokens:  ([ mudname -> ([ token -> username ]) ])
*/
static private mapping	returned_tokens = ([ ]);/* returned to us */
static private mapping	created_tokens = ([ ]);	/* we created and sent */


private nomask void handle_read(object socket, mixed * message);
private nomask void handle_close(object socket);


/*
** reconnect()
**
** Attempt a reconnect to the given mud
*/
private nomask void reconnect(string mud)
{
    object skt;
    string err;

    /*
    ** If a socket already exists for this mud, then just exit.  This
    ** implies a connection came in while the call_out() was pending.
    */
    if ( sockets[mud] || connecting[mud] )
	return;

DBBUG("connecting to "+mud);
    /*
    ** Catch any errors -- we want to ignore failures, not get killed
    ** by them.
    */
    err = catch(skt = new(SOCKET, SKT_STYLE_CONNECT_M,
			  muds[mud] + " " + mud_ports[mud],
			  (: handle_read :),
			  (: handle_close :)));
DBBUG(connecting[mud]);
    if ( err )
    {
	skt->remove();

	/*
	** Log the error and give up.  The socket creation should have
	** succeeded (we'll find out later if the connection succeeded).
	*/
	/* ### until we have good logging */
	DBBUG(err);
	error(err);
    }
    else
    {
	connecting[mud] = skt;

	/*
	** Bounce an echo off the other end.  When this comes back, we
	** will have an "established" connection and reset our retry
	** mechanism.
	*/
	err = catch(skt->send(({ "echo-req" })));
	/* ### until we have good logging */
	if (err)
	{
	    DBBUG(err);
	}
    }
}

/* maybe this should be in a mapping modules? */
private nomask mixed invert_mapping(mapping m, mixed value)
{
    int i = member_array(value, values(m));

    if (i == -1) return 0;
    return keys(m)[i];
}

private nomask string resolve_address(string address)
{
    string mud;
    string * names;
    int i;

    /* ### temp hack to resolve two muds at one IP */
    if ( address == muds[synonyms[mud_name()]] )
    {
	mapping work = copy(muds);
	map_delete(work, synonyms[mud_name()]);
	return invert_mapping(work, address);
    }

    mud = invert_mapping(muds, address);
    if ( mud )
	return mud;

    names = keys(dyn_muds);
    for ( i = sizeof(names); i--; )
    {
	string * pats = dyn_muds[names[i]];
	string * toks = allocate(sizeof(pats));	// bunch of zeroes

	if ( sizeof(reg_assoc(address, pats, toks)[0]) > 1 )
	    return names[i];
    }
}

private nomask void handle_read(object socket, mixed * message)
{
    string mud;

    if ( !message )
    {
	/* new connection */
        if ( !(mud = resolve_address(socket->address()[0])) )
	{
            /* not a legal client */
	    /* ### log the attempt? */

	    /* ### avoid MudOS bug: we're inside socket_release() */
	    call_out(0, (: call_other, socket, "remove" :));
	    return;	
        }

DBBUG("connect from: "+mud);
	/*
	** A legal mud has connected. If we already have a connection,
	** then we probably crossed lines.  We resolve which connection
	** is thrown out by a simple ordering on names.  The other end
	** will throw out one also (which should match the one we toss).
	*/
	if ( sockets[mud] || connecting[mud] )
	{
	    if ( mud < synonyms[mud_name()] )
	    {
		/* we throw out the session created by the remote end */
		/* ### avoid MudOS bug: we're inside socket_release() */
		call_out(0, (: call_other, socket, "remove" :));
		return;
	    }

	    /* we throw out the session that originated here */
	    if ( connecting[mud] )
	    {
		connecting[mud]->remove();
		map_delete(connecting, mud);
		cancel_reconnection(mud);
	    }
	    else
	    {
		sockets[mud]->remove();
	    }
	}

	/*
	** New connection.  Store it and reset the retry time.  If we
	** were busy trying to connect, then torch it.
	*/
        sockets[mud] = socket;
	returned_tokens[mud] = ([ ]);
	created_tokens[mud] = ([ ]);
    }
    else if ( message[0] == "call" )
    {
DBBUG(sprintf("call: %O", message[1..2]));
        unguarded(1, (: call_other, message[1..]... :));
//	apply((: call_other :), message[1..]);
    }
    else if ( message[0] == "auth-call" )
    {
DBBUG(sprintf("auth-call: %O", message[1..4]));
//### validate message[1] (token)
        unguarded(message[2], (: call_other, message[3..]... :));
    }
    else if ( message[0] == "eval" )
    {
	mixed result;

DBBUG(sprintf("eval: %O", message[1..3]));
        result = call_other(message[2..]...);
	socket->send(({ "result", message[1], result }));
    }
    else if ( message[0] == "auth-eval" )
    {
	mixed result;

DBBUG(sprintf("auth-eval: %O", message[1..5]));
//### validate message[2] (token)
	result = unguarded(message[3],
        (: call_other, message[4..]... :));
	socket->send(({ "result", message[1], result }));
    }
    else if ( message[0] == "result" )
    {
DBBUG(sprintf("result: %O", message[1..2]));
	evaluate(trans[message[1]], message[2]);
	map_delete(trans, message[1]);
    }
    else if ( message[0] == "echo-req" )
    {
DBBUG("echo-req");
	socket->send(({ "echo-reply" }));
    }
    else if ( message[0] == "echo-reply" )
    {
	/*
	** The connection is established.  Move the socket from connection[]
	** to sockets[].  Delete the retry information.
	*/
	mud = invert_mapping(connecting, socket);
DBBUG("echo-reply from: "+mud);
	sockets[mud] = socket;
	map_delete(connecting, mud);
	cancel_reconnection(mud);
	returned_tokens[mud] = ([ ]);
	created_tokens[mud] = ([ ]);
    }
    else if ( message[0] == "token-req" )
    {
	string	token;
	mapping	tkns;

	/*
	** ### validate user/pass: message[1], message[2]
	*/

	mud = invert_mapping(sockets, socket);
DBBUG(sprintf("token-req from %s for %s",mud,message[1]));
	tkns = created_tokens[mud];

	do
	{
	    token = sprintf("%08x%08x", random(0x7FFFFFFF), random(0x7FFFFFFF));
	} while ( tkns[token] );

	tkns[token] = message[1];

	socket->send(({ "token-reply", message[1], token}));
    }
    else if ( message[0] == "token-reply" )
    {
	object user;

	mud = invert_mapping(sockets, socket);
DBBUG(sprintf("token-reply from %s for %s",mud,message[1]));
	user = find_user(message[1]);
	returned_tokens[mud][user] = message[2];
    }
    else
    {
	error(sprintf("bad message received:\n%O\n", message));
    }
}

private nomask void handle_close(object socket)
{
    string mud = invert_mapping(sockets, socket);
    int delay;

    if ( !mud )
	mud = invert_mapping(connecting, socket);

DBBUG("closed: "+mud);
    /*
    ** When two connections open "simultaneously" we will close one.
    ** The socket that gets closed is bound to this function, but we
    ** did not record it, so we have to just return.
    */
    if ( !mud )
	return;

    map_delete(sockets, mud);
    map_delete(connecting, mud);
    map_delete(returned_tokens, mud);
    map_delete(created_tokens, mud);

    /*
    ** If the mud is not on our list of primary muds (one of the dynamic
    ** addresses), then don't try to reestablish contact -- assume that
    ** the dynamic mud will contact us.
    */
    if ( !muds[mud] )
    {
	DBBUG(mud + ": not on primary list... ignoring");

	cancel_reconnection(mud);
	return;
    }

    /*
    ** Get a reconnection to occur at the right time
    */
    trigger_reconnect(mud);
}

void create()
{
    string err;
    string *names;
    int i;

    if (clonep()) destruct(this_object());

    set_privilege(1);
    reconn_func = (: reconnect :);

    err = catch(our_listen_socket = new(SOCKET, SKT_STYLE_LISTEN_M,
					mud_ports[synonyms[mud_name()]],
					(: handle_read :)));
    if ( err )
    {
	DBBUG(err);
	our_listen_socket->remove();
	our_listen_socket = 0;
	error(err);
    }

    names = keys(muds) - ({ synonyms[mud_name()] });
    for ( i = sizeof(names); i--; )
    {
	/*
	** Start the connection process.
	*/
	trigger_reconnect(names[i]);
    }
}

void remove()
{
    (values(sockets) - ({ 0 }))->remove();
    (values(connecting) - ({ 0 }))->remove();

    if (our_listen_socket)
        our_listen_socket->remove();

    destruct(this_object());
}

//:FUNCTION remote_call
//handles the actully calling of remote functions on other muds.
void remote_call(string mud, string obj, string func, mixed *args)
{
    string syn = synonyms[mud];

    if ( !syn )
	error("Illegal contact mud: " + syn + ".\n");
    if ( !sockets[syn] )
	error("No connection to " + mud + " active.\n");

    /* This calls an arbitrary function, hence we need to be careful */    
    /* ### "call" is currently priv==1 */
    if ( !check_privilege(1) )
	error("Insufficient privilege.\n");

    sockets[syn]->send(({ "call", obj, func }) + args);
}

//:FUNCTION broadcast_call
//calls a remote function on all connected muds.
void broadcast_call(string obj, string func, mixed *args)
{
    /* This calls an arbitrary function, hence we need to be careful */
    /* ### "call" is currently priv==1 */
    if ( !check_privilege(1) )
        error("Insufficient privilege.\n");

    (values(sockets) - ({ 0 }))->send(({ "call", obj, func }) + args);
}
    
//:FUNCTION remote_eval
//calls a remote function and returns the result to a provided function.
void remote_eval(string mud, string obj, string func, mixed *args,
		  function result_func)
{
    string syn = synonyms[mud];
    int key;

    if ( !syn )
	error("Illegal contact mud: " + syn + ".\n");
    if ( !sockets[syn] )
	error("No connection to " + mud + " active.\n");

    /* NOTE: "eval" is priv==0. no need for priv check here */

    key = ++transid;
    trans[key] = result_func;

    sockets[syn]->send(({ "eval", key, obj, func }) + args);
}

//:FUNCTION broadcast_eval
//calls a remote func in all muds and calls the result_func for each
//returned result
void broadcast_eval(string obj, string func, mixed *args,
		    function result_func)
{
    object *s;
    int i;

    /* NOTE: "eval" is priv==0. no need for priv check here */

    s = values(sockets) - ({ 0 });
    for ( i = sizeof(s); i--; )
    {
	int key;

	key = ++transid;
	trans[key] = result_func;

	s[i]->send(({ "eval", key, obj, func }) + args);
    }
}

//:FUNCTION login_remote
//logs a user into to a remote mud
void login_remote(string mud, string password)
{
    string syn = synonyms[mud];
    string name;

    if ( !syn )
	error("Illegal contact mud: " + syn + ".\n");
    if ( !sockets[syn] )
	error("No connection to " + mud + " active.\n");

    /*
    ** ### do we need to do any validation the name?  I don't think
    ** ### it matters since it is the _remote_ mud that cares.
    */

    sockets[syn]->send(({ "token-req",
			      this_user()->query_real_name(),
			      password }));
}

//:FUNCTION is_logged_in
//returns whether a particular user is logged into the specified mud.
int is_logged_in(object user, string mud)
{
    if ( !returned_tokens[mud] )
	return 0;

    return returned_tokens[mud][user] != 0;
}

//:FUNCTION auth_call
//perform an authenticated/privileged call to a remote mud, carrying over
//the current (stack-based) privilege level.
void auth_call(string mud, string obj, string func, mixed *args)
{
    string syn = synonyms[mud];
    string token;
    mixed priv;

    if ( !syn )
	error("Illegal contact mud: " + syn + ".\n");
    if ( !sockets[syn] )
	error("No connection to " + mud + " active.\n");
    if ( !this_user() )
	error("No current user to enable access\n");
    if ( !(token = returned_tokens[mud][this_user()]) )
	error("Not logged into " + mud + ".\n");

    /* ### for now, require priv==1, and pass 1 */
    if ( !check_privilege(1) )
	error("Insufficient privilege.\n");
    priv = 1;

    sockets[syn]->send(({ "auth-call", token, priv, obj, func }) + args);
}

//:FUNCTION auth_eval
//perform an authenticated/privileged evaluation (and result return) on
//a remote mud, carrying over the current (stack-based) privilege level.
void auth_eval(string mud, string obj, string func, mixed *args,
	       function result_func)
{
    string syn = synonyms[mud];
    string token;
    mixed priv;
    int key;

    if ( !syn )
	error("Illegal contact mud: " + syn + ".\n");
    if ( !sockets[syn] )
	error("No connection to " + mud + " active.\n");
    if ( !this_user() )
	error("No current user to enable access\n");
    if ( !(token = returned_tokens[mud][this_user()]) )
	error("Not logged into " + mud + ".\n");

    /* ### for now, require priv==1, and pass 1 */
    if ( !check_privilege(1) )
	error("Insufficient privilege.\n");
    priv = 1;

    key = ++transid;
    trans[key] = result_func;

    sockets[syn]->send(({ "auth-eval", key, token, priv, obj, func }) + args);
}

//:FUNCTION muds_available
//returns information about the muds in the net, their connection status,
//and which this_user() has logged into.
mixed * muds_available()
{
    string * all_muds;
    mixed * result;
    int i;

    all_muds = keys(muds) + keys(dyn_muds) - ({ synonyms[mud_name()] });
    result = allocate(sizeof(all_muds));

    for ( i = sizeof(all_muds); i--; )
    {
	string mud = all_muds[i];
	int state;
	int logged_in;

	if ( sockets[mud] )
	    state = 2;
	else if ( connecting[mud] )
	    state = 1;
	else
	    state = 0;

	if ( state == 2 && this_user() &&
	    returned_tokens[mud][this_user()] )
	    logged_in = 1;
	else
	    logged_in = 0;

	result[i] = ({ mud, state, logged_in });
    }

    return result;
}


//:FUNCTION query_local_name
//returns the 'name' of the local mud
string query_local_name()
{
    return synonyms[mud_name()];
}


int stat_me()
{
    write("\nSOCKETS\n");
    (values(sockets)-({0}))->stat_me();

    write("\nCONNECTING\n");
    (values(connecting)-({0}))->stat_me();

    write("\nLISTEN: ");
    if ( our_listen_socket )
	our_listen_socket->stat_me();
    else
	write("<none>\n");

    reconnect::stat_me();

    printf("\nTRANS OUTSTANDING (transid=%d)\n%O\n", transid, trans);

    if ( check_privilege(1) )
    {
	printf("\nTOKENS PASSED OUT\n%O\n", created_tokens);
	printf("\nTOKENS RECEIVED\n%O\n", returned_tokens);
    }

    return 1;
}
