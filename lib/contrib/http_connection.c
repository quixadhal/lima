/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * Instructions for use:
 *
 * (1) add the following line to the driver config file:
 *     external_port_2 : binary 2080
 * 
 * (2) move this file to /obj/http_connection.c
 *
 * (3) disable /daemons/http_d.c
 *
 * (4) add the following to the top of connect() in master.c:
 *     object connect(int port) {
 *         object ob;
 *         string ret;
 *
 *         if (port == HTTP_PORT)
 *             return new("/obj/http_connection");
 *
 *         // rest of connect()
 *
 */ 

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
 * Nov 4 96 Beek: converted to connection style
 *
 * TODO: 
 *      * Hit logging and related stats
 *      * Support for multiple arguments to cgis
 *      * Support for large (larger than max buffer size) files
 *      * Better Error handling (only 404 so far ;) )
 */

/*
 * Instructions for use:
 *
 * (1) add the following line to the driver config file:
 *     external_port_2 : binary 2080
 * 
 * (2) move this file to /obj/http_connection.c
 *
 * (3) disable /daemons/http_d.c
 *
 * (2) add the following to the top of connect() in master.c:
 *     object connect(int port) {
 *         object ob;
 *         string ret;
 *
 *         if (port == HTTP_PORT)
 *             return new("/obj/http_connection");
 *
 *         // rest of connect()
 *
 */ 

#include <net/http_d.h>

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

private nomask void process_input(buffer data) {
    string file, args, request;

    // initial connect
    request = read_buffer(data);
    if ( !request )
	return;

    // We arent doing anything with the HTTP version  
    if ( sscanf(request, "GET %s %*s", file) != 2 )
        return;
    sscanf(file, "%s?=%s", file, args);
    file = convert_to_actual_path(file);
    if ( file_size(file) < 1 ) {
        receive("<title>404 Not found</title><h1>404 Not found<h1>"
		"<p>The requested URL was not found\n");
    } else
    if (file[<1..<2] == ".c") {
        string result, err;

	err = catch(result = file->main(args));

	if ( !result )
	    result = "<title>ERROR</title><h1>Error in gateway</h1>" + err;

	receive(result+"\n");
    } else
	receive(read_buffer(file));

    destruct(this_object());
}
