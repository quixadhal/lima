/* Do not remove the headers from this file! see /USAGE for more info. */

// Add Guest Command for Admins
// Vette April 4, 1997
//

//:ADMINCOMMAND
//USAGE: addguest [name]
//
// This command adds a guest to the guestlist for when the mud
// does not allow new players

#include <log.h>

inherit CMD;

#define SYNTAX	"Usage: addguest <name>\n"

void create()
{
    ::create();
    no_redirection();
}

private void main( string arg )
{

    if ( !check_privilege(1) )
	error("Must be an admin to use addguest.\n");

    if( !sizeof( arg ))
    {
	write( SYNTAX );
	return;
    }
    GUEST_D->add_guest(arg);
    write("Guest "+arg+" added.\n");
}
