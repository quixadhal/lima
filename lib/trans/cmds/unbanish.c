/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz

#include <daemons.h>
#include <mudlib.h>

inherit DAEMON;



int main( string arg )
{
    string * parts;
    
    if(!stringp(arg) || strsrch(arg," ") != -1)
	return 0;

    if( strsrch(arg,".") == -1 )
	return BANISH_D->unbanish_name( arg ), write("Done.\n"), 1;

    parts = explode(arg,".");

    if ( sizeof(parts) != 4 ||
	 sizeof(filter_array(parts,
			     (: $1 != "*" && $1 != "0" && to_int($1) :) )))
	return write("Bad format for site.\n"), 1;
    BANISH_D->unbanish_site(parts);

    write("Done.\n");
    return 1;
}

