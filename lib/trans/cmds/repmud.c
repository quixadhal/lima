/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** repmud.c -- replicate file to connected muds
**
** The user must be logged into all connected muds.  The specified
** file will be copied to those muds (to the same filesystem location).
** for a change comment.  The RCS daemon will alter the lock state, record
** the change, and replicate the file to the distributed muds.
** 
** 15-Apr-95.  Deathblade.  Created.
*/

#include <mudlib.h>
#include <daemons.h>

inherit DAEMON;		/* for privs and cleanup */
inherit M_REMOTE;

int main(mixed * arg)
{
    mixed * info;
    int i;
    string fname;
    string contents;

    /*
    ** Make sure the user is logged into all available muds.
    */
    info = muds_available();
    if ( sizeof(filter_array(info, (: $1[1] == 2 && !$1[2] :))) )
    {
	write("You should be logged into all connected muds.\n");
	return 1;
    }

    write("Replicating to: " +
	  implode(map_array(info, (: $1[0] :)), ", ") +
	  "\n");

    for ( i = 0; i < sizeof(arg[0]); ++i )
    {
	fname = arg[0][i];

	contents = read_file(fname);
	if ( !contents )
	{
	    printf("Problems reading: %s\n", fname);
	    return 1;
	}

	/* ### this will need to change to an authenticated version */
	call_broadcast(FILE_D, "perform_write_file", ({ fname, contents, 1 }));
	printf("  File '%s' sent.\n", fname);
    }

    return 1;
}
