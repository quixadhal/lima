/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** rls.c -- remote ls
**
** Print out a directory listing from a remote mud
**
** 15-Apr-95.  Deathblade.  Created.
*/

#include <mudlib.h>

inherit CMD;
inherit M_REMOTE;

private void print_it(mixed who, mixed result)
{
    tell_object(who,
		sprintf("Remote directory:\n%-#79s\n",
			implode(result, "\n")));
}

private void main(string arg)
{
    string mud;
    string dir;

    if ( sscanf(arg, "%s:%s", mud, dir) != 2 )
    {
	write("Usage: rls mud:dir\n");
	return;
    }

    eval_remote(mud, FILE_D, "perform_get_dir", ({ dir, 0 }),
		(: print_it, this_user() :));

    return;
}
