/* Do not remove the headers from this file! see /USAGE for more info. */

// Remove Guest Command for Admins
// Vette April 4, 1997
//

//:ADMINCOMMAND
//USAGE: removeguest <name>
//
//This command removes a guest from to the guestlist for when the mud
//does not allow new players
//
//Admin only

#include <log.h>

inherit CMD;

#define SYNTAX	"Usage: removeguest <name>\n"

void create()
{
  ::create();
  no_redirection();
}

private void main( string arg )
{
  if ( !check_privilege(1) )
    error("Must be an admin to use removeguest.\n");

  if( !sizeof( arg ))
  {
    write( SYNTAX );
    return;
  }
  GUEST_D->remove_guest(arg);
  write("Guest "+arg+" removed.\n");
}
