/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;

inherit M_GRAMMAR;


#define SYNTAX	"Usage: wall <message>\n"

int main( string arg )
{
    string		broadcast;

    if ( !check_privilege(1) )
	error("Must be an admin to use wall.\n");

    if(!arg)
	return write( SYNTAX ), 1;

    broadcast = sprintf("System message from %s at %s:\n    %s\n",
			this_body()->query_name(), ctime(time()),
			iwrap(punctuate(arg)) );

    write_file("/log/walls", broadcast );
    shout( broadcast );
    return 1;
}
