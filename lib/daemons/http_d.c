/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * Originally the utility port handler for the i3 router 
 * Cowl@Orion
 *
 * 11-19-95 Cowl: Added args for gateways
 * 12-27-95 Cowl: added home dirs for wizards and converted
 *                to a web server ( almost )
 * 01-05-96 Cowl: Converted socket to a binary, for graphics
 *                save for a minor bug, convert_to_actual_path() works.
 * 01-07-96 Cowl: fixed convert_to_actual_path()
 *                added gateway security 
 * TODO: 
 *      * Hit logging and related stats
 *      * Support for multiple arguments to cgis
 *      * Support for large (larger than max buffer size) files
 *      * Better Error handling (only 404 so far ;) )
 *
 * Jan 20, 1996 Myth@Icon of Sin
 *  o Added a write callback function for large files.
 *
 */

#include <socket.h>
#include <http_d.h>
#include <log.h>
#include <ports.h>

private static mapping sending=([]);
private static object http_sock;

// figures out what is being requested, and how to get there
private nomask string convert_to_actual_path(string path) {
    string file;
    if(path == "/")
        return HTTP_ROOT+"/"+DEFAULT_PAGE;
    if(path[<1] == '/')
        path = path[0..<2];
    if(path[<1] == 'c') /* this is ugly, but works for now */
        file = SECURE_CGI_DIR+"/"+explode(path, "/")[<1];
    else {
        if(path[1] == '~') {
            string user;
            if(sscanf(path[2..], "%s/%s", user, file) == 2)
                file = WIZ_DIR+"/"+user+HTTP_USER_HOME+"/"+file;
            else
                file = WIZ_DIR+"/"+path[2..]+HTTP_USER_HOME;
        }
        else 
            file = HTTP_ROOT+path;
    }
    /* if its a dir, so we want the default page */
    if(file_size(file) == -2) 
        (file[<1] == '/') ? file += DEFAULT_PAGE : file += "/" + DEFAULT_PAGE;
    return file;
}

private buffer write_callback(object socket) {
 string file;
 int i1,i2,i3;

 if (!sending[socket]) {
  socket->remove();
  return allocate_buffer(0);
 }

 file=sending[socket][0];
 i1=sending[socket][1];

 i2=HTTP_BLOCK_SIZE;

 i3=file_size(file);

 // In case the file gets deleted in the middle of a transfer.
 if (i3<0) {
  socket->remove();
  return allocate_buffer(0);
 }

 if (i1+i2>file_size(file)) {
  i2=file_size(file)-i1;
  map_delete(sending,socket);
 } else {
  sending[socket]=({file,i1+i2});
 }

 return read_buffer(file,i1,i2);
}

private nomask void http_read(object socket, buffer data) {
    string ver, file, extention, args, request;
    int i1;

    if (!data) return;
    // initial connect
    request = read_buffer(data);
    if ( !request )
	return;

    // We arent doing anything with the HTTP version  
    if ( sscanf(request, "GET %s %s", file, ver) !=2 )
        return;
    sscanf(file, "%s?=%s", file, args);
    file = convert_to_actual_path(file);

    if ( file_size(file) < 1 ) {
        socket->send("<title>404 Not found</title><h1>404 Not found<h1>"+
                     "<p>The requested URL was not found\n");
        socket->remove();
        return;
    }
    sscanf(file, "%s.%s", file, extention);
    // if it's extention is '.c' then we assume it is a gateway and
    // we send the output of the gateway object's main() to the client.
    // if the extention is anything other than '.c' we'll treat it as
    // an HTML document. gateways can only be called from a secure dir.
    switch(extention) {
        string result, err;
        case "c":
            if(args)
	        err = catch(result = call_other(file, "main", args));
            else
	        err = catch(result = call_other(file, "main"));
	    if ( !result )
	        result = "<title>ERROR</title><h1>Error in gateway</h1>" + err;
	    socket->send(result+"\n");
       	    socket->remove();
	    break;

        default:
            sending[socket]=({ file+"."+extention,0});
            socket->set_write_callback( (:write_callback:) );
            socket->send(write_callback(socket));
            break;
    }
}

private nomask void http_close(object socket) {
    // add logging or something later 
}

nomask void remove() {
    http_sock->remove();
}

nomask void create() {
    string ret;
    ret = catch(http_sock = new(SOCKET, SKT_STYLE_LISTEN_B, PORT_HTTP, 
                                (: http_read :), (: http_close :)));
    if ( ret ) {
        if ( http_sock ) {
            http_sock->remove();
            http_sock = 0;
        }
        error(ret);
    }
}
//**  added to stop the daemon from cleaning itself up

void clean_up()
{
   return 0;
}
