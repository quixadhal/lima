/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <log.h>

inherit CMD;
inherit M_GRAMMAR;


#define SYNTAX	"Usage: wall <message>\n"

private void main( string arg )
{
    string		broadcast;

    if ( !check_privilege(1) )
	error("Must be an admin to use wall.\n");

    if(!arg) {
	write( SYNTAX );
	return;
    }

    broadcast = sprintf("System message from %s at %s:\n    %s\n",
			this_body()->query_name(), ctime(time()),
			iwrap(punctuate(arg)) );

    LOG_D->log(LOG_WALL, broadcast);

    shout( broadcast );
    write(broadcast);
}
