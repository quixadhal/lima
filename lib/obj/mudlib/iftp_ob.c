/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** iftp_ob.c -- interactive object to handle I3 file transfers
**
** 960205, Deathblade: created
*/


inherit M_INPUT;

private static string remote_mudname;
private static string remote_dir = "/";

private nomask string local_dir()
{
    return get_user_variable("pwd") || DIR_I3_FILES;
}

private nomask void rcv_command(string str)
{
    string args;
    string local_fname;
    string remote_fname;

    if ( str == "" )
	return;

    sscanf(str, "%s %s", str, args);
    switch ( str )
    {
    case "quit":
    case "q":
    case "bye":
    case "exit":
	modal_pop();
	destruct();
	break;

    case "?":
    case "help":
    default:
	write("Commands are: quit, help, get, put, ls, cd, lcd, pwd\n");
	break;

    case "get":
	if ( !args )
	{
	    write("Error: usage is: get remote_fname [ local_fname ]\n");
	    break;
	}
	sscanf(args, "%s %s", remote_fname, local_fname);
	if ( !remote_fname )
	    local_fname = remote_fname = args;
	if ( local_fname[0] != '/' )
	    local_fname = join_path(local_dir(), local_fname);
	if ( remote_fname[0] != '/' )
	    remote_fname = join_path(remote_dir, remote_fname);
	IMUD_D->do_file_get_request(remote_mudname, local_fname, remote_fname);
	break;

    case "put":
	if ( !args )
	{
	    write("Error: usage is: put local_fname [ remote_fname ]\n");
	    break;
	}
	sscanf(args, "%s %s", local_fname, remote_fname);
	if ( !local_fname )
	    local_fname = remote_fname = args;
	if ( local_fname[0] != '/' )
	    local_fname = join_path(local_dir(), local_fname);
	if ( remote_fname[0] != '/' )
	    remote_fname = join_path(remote_dir, remote_fname);
	IMUD_D->do_file_put_request(remote_mudname, local_fname, remote_fname);
	break;

    case "ls":
    case "dir":
	if ( !args )
	    args = remote_dir;
	else if ( args[0] != '/' )
	    args = join_path(remote_dir, args);
	IMUD_D->do_file_list_request(remote_mudname, args);
	break;

    case "cd":
	if ( !args )
	    printf("Dir: %s\n", remote_dir);
	else
	{
//### validate the dir?
	    remote_dir = args;
	    printf("New dir: %s\n", args);
	}
	break;

    case "pwd":
	printf("Local:  %s\nRemote: %s\n", local_dir(), remote_dir);
	break;

    case "lcd":
	if ( !args )
	    printf("Dir: %s\n", local_dir());
	else
	{
//### validate the dir?
	    this_user()->query_shell_ob()->set_pwd(args);
	    printf("New dir: %s\n", args);
	}
	break;
    }
}

void create(string mudname)
{
    if ( !clonep() )
	return;
    if ( !mudname || mudname == "" )
	error("illegal creation of an IFTP_OB\n");

    printf("Accessing: %s\n", IMUD_D->canon_mudname(mudname));
    remote_mudname = mudname;

    modal_push((: rcv_command :), "iftp> ");
}
