/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** rcs_d.c
**
** Source control daemon.  Handles source control for the mud and
** coordinating within a distributed net of muds
*/

#include <config.h>
#include <mudlib.h>
#include <daemons.h>
#include <security.h>

inherit M_ACCESS;
inherit M_REMOTE;

DOC_MODULE("This daemon manages the source control system's database.");

#define SAVE_FILE	"/data/daemons/rcs_d"
#define LOG_FILE	"/log/rcs"	/* ### move to a logging daemon? */
#define HOLDING_DIR	"/data/daemons/rcs/"
#define SYNC_STAMP	"..sync.."

/*
** This array specifies all the directories that are controlled by
** this daemon.  They have trailing slashes.
*/
private string * controlled = ({ });

/*
** Map pathnames to a mapping that maps filenames to information about
** that file.  The information is a 4 element array: ({ version number,
** locked-by, mud-locked-on, master-vsn number }).
**
** Each directory's mapping will also have an entry keyed with the
** SYNC_STAMP symbol.  It's value will be the synchronization value
** for that directory.
*/
private mapping info = ([ ]);	/* path -> fname -> info */

/*
** This token is issued by the server when it has synchronized the
** client.  The client will clear it out whenever a change occurs,
** and the server generates new ones when a change occurs.  This
** mechanism allows the client and server to detect potential
** synchronization failures.
*/
private int sync_token;

/*
** Files that need to be merged go to a holding area.  This mapping
** gives the original dir/fname/user/mud/svr_vsn info for a file in the
** holding area.
*/
private mapping holding_area = ([ ]);

/*
** Is this RCS_D the server?
*/
static private int is_server;


private nomask void write_log(string s)
{
    s = sprintf("%s: %s\n", ctime(time()), s);
    unguarded(1, (: write_file, LOG_FILE, s :));
}

private nomask void save_changes(string dir)
{
    /*
    ** Any time the server sees a change, the token is bumped
    ** up.  When a client sees a change, the token is reset.
    */
    if ( is_server )
	sync_token = time();
    else
	sync_token = 0;

    /*
    ** Also associate the token with the directory that was changed.
    */
    if ( dir )
	info[dir][SYNC_STAMP] = sync_token;

    unguarded(1, (: save_object, SAVE_FILE :));
}

void create()
{
    set_privilege(1);
    restore_object(SAVE_FILE);
    is_server = query_local_name() == RCS_SERVER;
}

nomask void add_directory(string dir)
{
    string * files;
    mapping finfo;

    if ( !check_privilege(1) )
	error("Insufficient privilege.\n");

    if ( member_array(dir, controlled) != -1 )
	return;
    if ( file_size(dir) != -2 )
	error("Not a directory: " + dir + "\n");

    controlled += ({ dir });

    files = map_array(get_dir(dir, -1),
		      (: $1[1] == -2 ? 0 : $1[0] :))
	- ({ 0 });

    finfo = ([ ]);
    map_array(files, (: $(finfo)[$1] = ({ 1, 0, 0, 0 }) :));
    info[dir] = finfo;

    save_changes(0);

    /*
    ** Get the other muds to add the directory, too
    */
    call_broadcast(RCS_D, "add_directory", ({ dir }));
}
nomask void remove_directory(string dir)
{
    if ( !check_privilege(1) )
	error("Insufficient privilege.\n");

    if ( member_array(dir, controlled) == -1 )
	return;

    controlled -= ({ dir });
    map_delete(info, dir);

    save_changes(0);

    /*
    ** Get the other muds to remove the directory
    */
    call_broadcast(RCS_D, "remove_directory", ({ dir }));
}
nomask string * query_directories()
{
    return copy(controlled);
}

nomask mixed * query_files(string dir)
{
    mapping dinfo;

    if ( !(dinfo = info[dir]) )
	error("Directory not under source control: " + dir + "\n");

    return map_array(keys(dinfo) - ({ SYNC_STAMP }),
		     (: ({ $1 }) + $(dinfo)[$1] :));
}

/*
** deliver_message()
**
** Deliver a message from the server to a user
** ### this should be more generic... move it somewhere else
*/
nomask void deliver_message(string username, string message)
{
    object p;

    p = find_body(username);
    if ( p )
	tell_object(p, message);
}


/*
** get_finfo()
**
** This must be private -- it returns a reference to the array stored
** in the "info" variable
*/
private nomask mixed * get_finfo(string dir, string fname)
{
    mapping dinfo;
    mixed * finfo;

    if ( !(dinfo = info[dir]) )
	error(sprintf("Directory not under source control: %s\n", dir));
    if ( !(finfo = dinfo[fname]) )
	error(sprintf("File not under source control: %s%s\n", dir, fname));

    return finfo;
}

nomask void do_check_out(string dir, string fname,
			 string username, string mudname)
{
    mixed * finfo;

    finfo = get_finfo(dir, fname);
    if ( finfo[1] )
    {
	write_log(sprintf("ERR: %s@%s failed to get %s%s/%s@%s",
			  username, mudname, dir, fname,
			  finfo[1], finfo[2]));
	error("File already checked out.\n");
    }

    finfo[1] = username;
    finfo[2] = mudname;

    save_changes(dir);

    /*
    ** Record the checked-out (and possibly vsn-converted) file
    */
    write_log(sprintf("%s: %s@%s - %s%s;%d;%d",
		      finfo[3] ? "CVO" : "OUT",
		      username, mudname, dir, fname, finfo[0], finfo[3]));
}

nomask void server_check_out(string dir, string fname,
			     string username, string mudname)
{
    /*
    ** Check the file out here on the server
    */
    do_check_out(dir, fname, username, mudname);

    /*
    ** Tell the other muds that it has been checked out
    */
    call_broadcast(RCS_D, "do_check_out",
		   ({ dir, fname, username, mudname }));

    /*
    ** Tell the user that the checkout is complete
    */
    call_remote(mudname, RCS_D, "deliver_message",
		({ sprintf("'%s' checked out.\n", fname) }));
}


/*
** server_connected()
**
** Is the server connected and this_user() logged in?  Return one of:
**	0 - not connected
**	1 - connected, but not logged in
**	2 - connected and logged in
*/
private nomask int server_connected()
{
    mixed * muds;

    muds = filter_array(muds_available(), (: $1[0] == RCS_SERVER :));
    if ( sizeof(muds) != 1 )
	return 0;

    muds = muds[0];
    return muds[1] != 2 ? 0 : muds[2] + 1;
}

nomask void check_out_file(string dir, string fname)
{
    mixed * finfo;
    string username;
    string thismud;

    finfo = get_finfo(dir, fname);
    if ( finfo[1] )
	error("Already checked out by: " + finfo[1] + "\n");

    /*
    ** We don't actually need to perform any operations at this privilege,
    ** but the user should have it, indicating they are allowed to change
    ** the file in question.
    */
    if ( !check_privilege(get_protection(dir, "wd")) )
	error("Insufficient privilege\n");

    username = this_user()->query_real_name();
    thismud = query_local_name();
    if ( is_server )
    {
	server_check_out(dir, fname, username, RCS_SERVER);

	printf("'%s' checked out.\n", fname);
    }
    else
    {
	/*
	** Ask the server to check it out.  It will, in turn, send to
	** all muds a notification of the new status.  We'll respond
	** to that appropriately.
	*/
	if ( server_connected() > 0 )
	{
	    call_remote(RCS_SERVER, RCS_D, "server_check_out",
			({ dir, fname, username, thismud }));
	}
	else
	{
	    /*
	    ** The server is not connected.  Perform a local checkout --
	    ** possibly save away the server's vsn number and start a new
	    ** vsn number system that is local.
	    **
	    ** The file will have to be sync'd up later.
	    */
	    if ( !finfo[3] )
	    {
		finfo[3] = finfo[0];	/* save server's vsn num */
		finfo[0] = 1;
	    }

	    do_check_out(dir, fname, username, thismud);

	    printf("'%s' checked out (FORKED LOCAL COPY).\n", fname);
	}
    }
}


nomask void remote_set_finfo(string dir, string fname, int vsn,
			     string username, string mudname, int svr_vsn)
{
    mixed * finfo;

    finfo = get_finfo(dir, fname);

    finfo[0] = vsn;
    finfo[1] = username;
    finfo[2] = mudname;
    finfo[3] = svr_vsn;

    save_changes(dir);
}

private nomask void local_check_in(string dir, string fname, int vsn,
				   string username, string mudname)
{
    mixed * finfo;

    finfo = get_finfo(dir, fname);

    /*
    ** Allow a put when there is no lock (this will happen if we were
    ** not connected to the server when somebody checked out a file).
    **
    ** If there is a lock, but it differs, then a put is not allowed.
    */
    if ( finfo[1] && (finfo[1] != username || finfo[2] != mudname) )
    {
	write_log(sprintf("ERR: %s@%s failed to put %s%s;%d/%s@%s",
			  username, mudname, dir, fname, vsn,
			  finfo[1], finfo[2]));
	return;
    }

    /*
    ** Set the appropriate version and clear the other info
    */
    finfo[0] = vsn;
    finfo[1] = 0;
    finfo[2] = 0;
    finfo[3] = 0;

    save_changes(dir);

    /*
    ** Record the checkin
    */
    write_log(sprintf("IN : %s@%s - %s%s;%d",
		      username, mudname, dir, fname, finfo[0]));
}

nomask void remote_check_in(string dir, string fname, int vsn,
			    string username, string mudname,
			    string contents)
{
    local_check_in(dir, fname, vsn, username, mudname);

    /*
    ** No special privs.  Should be at the priv level that has been set
    ** up by the remote_d.
    */
    write_file(dir + fname, contents, 1);
}

private nomask void distribute_file(string dir, string fname,
				    string username, string mudname,
				    string contents,
				    mixed * mudinfo)
{
    /*
    ** If the mud isn't connected or the user is not logged into it,
    ** then do nothing.
    */
    if ( mudinfo[1] != 2 || !mudinfo[2] )
	return;

    /*
    ** Send the file there (username@mudname is checking it in)
    */
    auth_call(mudinfo[0], RCS_D, "remote_check_in",
	      ({ dir, fname, username, mudname, contents }));
}

nomask void server_check_in(string dir, string fname,
			    string username, string mudname,
			    int svr_vsn,
			    string contents)
{
    mixed * finfo;

    finfo = get_finfo(dir, fname);
    if ( svr_vsn != finfo[0] )
    {
	string pathname;

	/*
	** The file was forked off a different version than our current
	** version.  Place the file into the holding area for manual
	** merging.
	*/
	do
	{
	    pathname = sprintf("%shold%04x", HOLDING_DIR, random(65536));

	} while ( file_size(pathname) != -1 );

	unguarded(1, (: write_file, pathname, contents :));

	holding_area[fname] = ({ dir, fname, username, mudname, svr_vsn });

	write_log(sprintf("HLD: %s@%s forked %s%s;%d",
			  username, mudname, dir, fname, svr_vsn));

	/*
	** Tell the original mud the new file info (keep the same vsn
	** data, but unlock the file).
	*/
	call_remote(mudname, RCS_D, "remote_set_finfo",
		    ({ dir, fname, 1, 0, 0, svr_vsn }));
    }
    else
    {
	int vsn;

	/*
	** Check the file in locally (using remote_check_in() to do the
	** update and write the file) and then pass the file out to the
	** other muds.
	*/
	vsn = finfo[0] + 1;
	remote_check_in(dir, fname, vsn, username, mudname, contents);

	/*
	** No sense in even trying this.  The way this function was
	** invoked (via incoming socket data and a call from remote_d),
	** there will be no current player to define privileges at
	** the other muds.  The remote_d won't allow an authenticated
	** call; therefore, we couldn't update the file at the remote
	** muds.
	*/
#ifdef NOPE
	map_array(muds_available() - ({ mudname }),
		  (: distribute_file, dir, fname, vsn, username, mudname,
		   contents :));
#endif

	/*
	** Tell the original mud the new file info (new vsn, unlocked)
	*/
	call_remote(mudname, RCS_D, "remote_set_finfo",
		    ({ dir, fname, vsn, 0, 0, 0 }));
    }

    /*
    ** Tell the user that the checkin is complete
    */
    call_remote(mudname, RCS_D, "deliver_message",
		({ sprintf("'%s' checked in.\n", fname) }));
}

nomask void check_in_file(string dir, string fname)
{
    mixed * finfo;
    string username;
    string thismud;
    string contents;

    finfo = get_finfo(dir, fname);

    if ( !check_privilege(get_protection(dir, "wd")) )
	error("Insufficient privilege\n");

    username = this_user()->query_real_name();
    thismud = query_local_name();

    if ( !finfo[1] )
	error("Not checked out.\n");
    if ( finfo[1] != username )
	error(finfo[1] + " has it locked, not you.\n");
    if ( thismud != finfo[2] )
	error("Must be checked in on check-out mud (" + finfo[2] + ")\n");

    contents = read_file(dir + fname);
    if ( !contents )
	error("Could not read file");

    if ( is_server )
    {
	int vsn;

	/*
	** Check in locally and then distribute copies
	*/
	vsn = finfo[0] + 1;
	local_check_in(dir, fname, vsn, username, RCS_SERVER);

	map_array(muds_available(),
		  (: distribute_file, dir, fname, vsn, username, RCS_SERVER,
		   contents :));

	printf("'%s' checked in.\n", fname);
    }
    else if ( server_connected() != 2 )
    {
	error("Not connected to server (" + RCS_SERVER + ")\n");
    }
    else
    {
	/*
	** Send the file to the server.  Generally, it will direct us
	** to unlock the thing.
	**
	** We may have "checked it out" without actually telling the
	** server.  The server will handle it and will direct us
	** appropriately based on how it resolves the problem.
	*/
	auth_call(RCS_SERVER, RCS_D, "server_check_in",
		  ({ dir, fname, username, thismud, finfo[3], contents }));
    }
}
