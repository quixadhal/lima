/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>
#include <driver/origin.h>
#include <commands.h>

string *paths;

/*
** This would normally be a Bad Thing [tm] except that
** it is used in ONE place in this file.  The only reason to
** not have it right in the code at that one place is for
** clarity -- we are trying to show which paths are valid.
** Further, if we used a static private var, then EVERY user
** would get a copy of the array.  This way, we have a single
** copy of the array compiled into the program.  Now, of course,
** this comes with a caveat: if this is used more than once, then
** _maybe_ it should be a variable to prevent multiple copies.
** And yes, Beek may kill me for this, but we'll see... :-)
*/
#define VALID_PLAYER_PATHS ({ CMD_DIR_PLAYER "/" })

int query_privilege();
object query_link();

nomask void path_create()
{
    paths = ({ CMD_DIR_PLAYER "/" });
}

nomask static void nuke_paths()
{
    /* this function is called when a player is de-wizzed */
    /* this function needs security bad! */
    paths = ({ CMD_DIR_PLAYER "/" });
}

nomask string * query_path()
{
    if(!paths)
	nuke_paths();
    return paths;
}

nomask int add_path(string path)
{
    if( !check_privilege( query_privilege() ) )
	return -2;

    if( member_array(path, paths) != -1 )
	return -1;

    if( file_size( path ) == -1 )
	return -4;

    if( path[0] != '/' || ( file_size( path ) == -2 && path[<1] != '/' ) )
	return -3;

    if(!wizardp(query_link()) && !member_array(path,VALID_PLAYER_PATHS))
        return -5;

    paths = ({ path }) + paths;
    return 1;
}

nomask int remove_path(string path)
{
    if( !check_privilege( query_privilege() ) )
	return 0;

    if( path == CMD_DIR_PLAYER "/" || file_size( path ) == -1 ||
      path[0] != '/' || ( file_size( path ) == -2 &&
	path[<1] != '/' ) )
	return 0;

    paths -= ({ path });
    return 1;
}
