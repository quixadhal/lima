/* Do not remove the headers from this file! see /USAGE for more info. */

// Guest Query Command for Admins
// Vette April 4, 1997
//

//:COMMAND
// This command lists the names of guests allowed for when the mud
// does not allow new players
// Syntax: whoguests

#include <mudlib.h>
#include <log.h>

inherit CMD;

void create()
{
    ::create();
    no_redirection();
}

private void main( string arg )
{
    string *guests;
    int i;

    if ( !check_privilege(1) )
	error("Must be an admin to use whoguests.\n");

    guests = GUEST_D->query_guests(arg);
    guests = sort_array(guests,1);
    i = sizeof(guests);
    for (i=0; i<sizeof(guests); i++) {
      write(guests[i]+"\n");
    }
}
