/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** file.c -- handler for I3's "file" service
**
** 950126, Deathblade: created
*/

#include <log.h>

void oob_register_requests(mapping requests);
void oob_register_replies(mapping replies);
void oob_initiate_connection(string target_mudname);
void oob_svc_send(object socket, mixed * message);
void oob_svc_error(object socket, string errcode, string errmsg,
		   mixed * errpacket);

/*
** Store the lists of requests for each target mud
*/
nosave private mapping file_requests = ([ ]);

nosave private string * file_status = ({
    "request failed (write permission)",
    "request failed (read  permission)",
    "request failed (fpath error)",
    "request failed (unknown error)",
    "request successful"
});


private nomask string prepare_path(string path)
{
    if ( path == "/" )
	path = DIR_I3_FILES;
    else
    {
	string * parts;

	parts = explode(path, "/") - ({ "", "." });
	for ( int idx = 0; idx < sizeof(parts); )
	{
	    if ( parts[idx] == ".." )
	    {
		if ( idx )
		{
		    parts[idx-1..idx] = ({ });
		    --idx;
		}
		else
		    parts[idx..idx] = ({ });
	    }
	    ++idx;
	}
	path = DIR_I3_FILES;
	if ( sizeof(parts) )
	    path += "/" + implode(parts, "/");
    }
DBBUG(path);

    return path;
}

private nomask void file_add_request(string mudname, mixed * request)
{
    if ( !file_requests[mudname] )
	file_requests[mudname] = request;
    else
	file_requests[mudname] += request;
}

nomask void do_file_list_request(string mudname, string dir)
{
    file_add_request(mudname, ({ ({ "file-list-req",
					mud_name(),
					this_user()->query_userid(),
					dir,
					}) }));

    oob_initiate_connection(mudname);
}

nomask void do_file_put_request(string mudname,
				string local_fname,
				string remote_fname)
{
    file_add_request(mudname, ({ ({ "file-put",
					random(50000),
					mud_name(),
					this_user()->query_userid(),
					remote_fname,
					local_fname,
					}) }));

    oob_initiate_connection(mudname);
}

nomask void do_file_get_request(string mudname,
				string local_fname,
				string remote_fname)
{
    file_add_request(mudname, ({ ({ "file-get-req",
					random(50000),
					mud_name(),
					this_user()->query_userid(),
					remote_fname,
					local_fname,
					}) }));

    oob_initiate_connection(mudname);
}

private nomask mixed * handle_file_reply(string mudname,
					 object socket,
					 mixed * message,
					 string request_type,
					 string reply_type,
					 int request_id,
					 int reply_id,
					 int req_user_idx
					 )
{
    object p;
    mixed * requests = file_requests[mudname];
    mixed * request;

    if ( !sizeof(requests) || requests[0][0] != request_type )
    {
	oob_svc_error(socket,
		      "bad-proto",
		      "unexpected " + reply_type,
		      message);
	return 0;
    }
    request = requests[0];
    file_requests[mudname] = requests[1..];

    if ( request[request_id] != message[reply_id] )
    {
	oob_svc_error(socket,
		      "bad-proto",
		      reply_type + " did not match the " + request_type,
		      message);
	return 0;
    }

    p = find_user(request[req_user_idx]);

    return ({ p, request });
}

private nomask void handle_file_list_req(string mudname,
					 object socket,
					 mixed * message)
{
    string dir;
    mixed * data;
    string err;

    dir = message[3];
    if ( !sizeof(dir) || !stringp(dir) )
    {
	oob_svc_error(socket,
		      "bad-proto",
		      "bad directory specified",
		      message);
	return;
    }

    dir = prepare_path(dir);
    err = catch(data = get_dir(dir + "/*", -1));
    if ( err )
    {
	oob_svc_error(socket,
		      "bad-directory",
		      err,
		      ({ }));
	return;
    }

    oob_svc_send(socket, ({ "file-list-reply", message[2], data }));
}

private nomask void handle_file_list_reply(string mudname,
					   object socket,
					   mixed * message)
{
    mixed * info = handle_file_reply(mudname,
				     socket,
				     message,
				     "file-list-req",
				     "file-list-reply",
				     2, 1, 2);
    string list;

    if ( !info || !info[0] )
	return;

    if ( message[2] )
    {
	list = implode(message[2],
		       (: sprintf("%s%-20s %8s   %s\n",
				  $1,
				  $2[0],
				  $2[1] == -2 ? "- DIR -" : ""+$2[1],
				  ctime($2[2])) :),
		       "");
    }
    else
    {
	list = "<< an error occurred on the target mud >>\n";
    }
    info[0]->receive_private_msg(sprintf("\nDirectory %s on %s:\n%s\n",
					 info[1][3],
					 mudname, list));
}

private nomask void handle_file_put(string mudname,
				    object socket,
				    mixed * message)
{
    string fname;
    string err;

    fname = message[4];
    if ( !sizeof(fname) || !stringp(fname) )
    {
	oob_svc_send(socket, ({ "file-put-ack", message[1], -1 }));
	return;
    }

    fname = prepare_path(fname);
    err = catch(write_file(fname, message[5], 1));
    if ( err )
    {
	oob_svc_send(socket, ({ "file-put-ack", message[1], 0 }));
	return;
    }

    oob_svc_send(socket, ({ "file-put-ack", message[1], 1 }));
}

private nomask void handle_file_put_ack(string mudname,
					object socket,
					mixed * message)
{
    mixed * info = handle_file_reply(mudname,
				     socket,
				     message,
				     "file-put",
				     "file-put-ack",
				     1, 1, 3);
    string msg;
    int status;

    if ( !info || !info[0] )
	return;

    status = message[2];
    if ( status < -3 || status > 1 )
	status = 3;
    else
	status += 3;

    msg = sprintf("\n[IFTP] %s put of %s: %s\n",
		  mudname,
		  info[1][5],
		  file_status[status]);
    info[0]->receive_private_msg(msg);
}

private nomask void handle_file_get_req(string mudname,
					object socket,
					mixed * message)
{
    string fname;
    string err;
    string contents;

    fname = message[4];
    if ( !sizeof(fname) || !stringp(fname) )
    {
	oob_svc_send(socket, ({ "file-get-reply", message[1], -1, 0 }));
	return;
    }

    fname = prepare_path(fname);
    err = catch(contents = read_file(fname));
    if ( err )
    {
	oob_svc_send(socket, ({ "file-get-reply", message[1], 0, 0 }));
	return;
    }

    oob_svc_send(socket, ({ "file-get-reply", message[1], 1, contents }));
}

private nomask void handle_file_get_reply(string mudname,
					  object socket,
					  mixed * message)
{
    mixed * info = handle_file_reply(mudname,
				     socket,
				     message,
				     "file-get-req",
				     "file-get-reply",
				     1, 1, 3);
    string msg;
    int status;

    if ( !info )
	return;

    status = message[2];
    if ( status < -3 || status > 1 )
	status = 3;
    else
	status += 3;
    msg = sprintf("\n[IFTP] %s get of %s (to %s): %s\n",
		  mudname,
		  info[1][4],
		  info[1][5],
		  file_status[status]);

    if ( status == 4 )
    {
	string err;

	err = catch(write_file(info[1][5], message[3], 1));
	if ( err )
	    msg = sprintf("\n[IFTP] %s get of %s: ERROR: %s\n",
			  mudname,
			  info[1][5],
			  err);
    }

    if ( info[0] )
	info[0]->receive_private_msg(msg);
}

protected nomask int file_has_outgoing(string mudname)
{
    return sizeof(file_requests[mudname]) != 0;
}

protected nomask int file_send_outgoing(string mudname, object socket)
{
    mixed * requests = file_requests[mudname];
    mixed * request;

    if ( !sizeof(requests) )
	return 0;
    request = requests[0];

    switch ( request[0] )
    {
    case "file-list-req":
	oob_svc_send(socket, request);
	break;
    case "file-put":
	oob_svc_send(socket,
		     request[0..4] + ({ read_file(request[5]) }));
	break;
    case "file-get-req":
	oob_svc_send(socket, request[0..4]);
	break;
    }
}

protected nomask void file_startup()
{
    oob_register_requests(([ "file-list-req" : (: handle_file_list_req :),
				 "file-put" : (: handle_file_put :),
				 "file-get-req" : (: handle_file_get_req :),
				 ]));
    oob_register_replies(([ "file-list-reply" : (: handle_file_list_reply :),
				"file-put-ack" : (: handle_file_put_ack :),
				"file-get-reply" : (: handle_file_get_reply :),
				]));
}
