/* Do not remove the headers from this file! see /USAGE for more info. */

// This space intentionally left blank
// Megaboz@ZorkMUD attached header and help

#include <commands.h>

int main(string arg)
{
    string   target;
    string	tmp, tmp2;

    if(!arg || arg == "")
      {
	write("Usage: reply <message>\n");
	return 1;
      }
    target = this_body()->query_reply();

    if( !find_user( target ) && !sscanf( target, "%s@%s", tmp, tmp2 ) )
    { 
	write( "No target found for reply.\n" );
	return 1;
    }

    CMD_OB_TELL->main(sprintf("%s %s", target, arg));
    return 1;
}
