/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** xfertool.c
**
** Instances of this class are used to transfer files between muds.
** Effectively, a very primitive FTP mechanism.  One advantage is that
** the listen port is short-lived and the tool takes on the privileges
** of its creator (as opposed to an FTPD which must have the highest
** priv).  Files can only be placed where the creator has access.
**
** At its most basic level, this object creates a TCP session with a
** similar object on another mud.
**
** 09-Apr-95. Deathblade. Created.
*/

#include <mudlib.h>
#include <security.h>
#include <socket.h>

inherit OBJ;
inherit M_ACCESS;
inherit M_GETTABLE;

static private object	creator;	/* who created this tool */
static private object	listen;		/* listening socket (incoming) */
static private object	connect;	/* connection socket (outgoing) */

string build_long()
{
    string res;

    if ( !creator )
    {
	remove();
	return "";
    }

    res = "A funky little wizard tool for transferring files.\n";
    res += "It is owned by: " + creator->query_name() + "\n";
    if ( listen )
	res += sprintf("It is listening with socket: %O\n", listen);
    if ( connect )
	res += sprintf("It is connected with socket: %O\n", connect);

    return res;
}

void create()
{
    ::create();

    set_in_room_desc("There is a transfer tool here.");
    set_weight(4);
    set_id( ({ "xfertool", "tool", "transfer" }) );
    set_long((: build_long :));
    set_gettable(1);
    set_dropable(1);

    if ( !clonep() )
	return;

    creator = this_user();
    set_privilege(get_privilege(creator));
}

private nomask void skt_incoming(object skt, mixed data)
{
    if ( !data )
    {
	if ( connect )
	    connect->remove();
	connect = skt;
	tell_object(creator, sprintf("Connection came in: %O\n", skt));
    }
    else if ( mapp(data) )
    {
	switch ( data[1] )
	{
	    mixed	priv;
	    string	fname;

	case 1:		/* file transfer */
	    fname = data[4] ? data[4] : data[2];

	    /*
	    ** There is no this_user(), so we must insert an unguarded
	    ** somewhere in the stack (only a socket is higher in the
	    ** stack and it does not do an unguarded).  Request the
	    ** appropriate privilege.
	    */
	    priv = get_privilege(this_object());
	    unguarded(priv, (: write_file, fname, data[3], 1 :));
	    tell_object(creator, sprintf("Rcvd file: %s (%d bytes)\n",
					 fname, sizeof(data[3])));
	    break;

	default:
	    tell_object(creator, sprintf("Rcvd: %O\n", data));
	}
    }
    else
    {
	tell_object(creator, sprintf("Rcvd: %O\n", data));
    }
}

private nomask void skt_closed(object skt)
{
    if ( skt == listen )
    {
	listen = 0;
	tell_object(creator, sprintf("Listen socket closed: %O\n", skt));
    }
    else if ( skt == connect )
    {
	connect = 0;
	tell_object(creator, sprintf("Connect socket closed: %O\n", skt));
    }
    else
    {
	tell_object(creator, sprintf("Unknown socket closed: %O\n", skt));
    }
}

nomask void send(mixed message)
{
    if ( connect )
	connect->send(message);
}

varargs nomask void send_file(string fname, string targetname)
{
    string contents;

    if ( !connect )
    {
	printf("Problems reading: %s\n", fname);
	return;
    }

    contents = read_file(fname);
    if ( !contents )
    {
	printf("Problems reading: %s\n", fname);
	return;
    }

    send(([ 1:1, 2:fname, 3:contents, 4:targetname ]));
}

nomask void listen(int port)
{
    if ( !clonep() )
	return;

    if ( listen )
	listen->remove();

    listen = clone_object(SOCKET, SKT_STYLE_LISTEN_M,
			  port, (: skt_incoming :), (: skt_closed :));
    printf("Listening at: %d\n", port);
}

nomask void connect(string addr)
{
    if ( !clonep() )
	return;

    if ( connect )
	connect->remove();

    connect = clone_object(SOCKET, SKT_STYLE_CONNECT_M,
			   addr, (: skt_incoming :), (: skt_closed :));
    printf("Connected to: %s\n", addr);
}

void remove()
{
    if ( listen )
	listen->remove();
    if ( connect )
	connect->remove();
    ::remove();
}
