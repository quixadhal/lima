/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz

#include <daemons.h>
#include <mudlib.h>

inherit CMD;



private void main( string arg )
{
    string * parts;
    
    if(!stringp(arg) || strsrch(arg," ") != -1)
	return;

    if( strsrch(arg,".") == -1 ) {
	BANISH_D->unbanish_name( arg );
	write("Done.\n");
	return;
    }
    
    parts = explode(arg,".");

    if ( sizeof(parts) != 4 ||
	 sizeof(filter_array(parts,
			     (: $1 != "*" && $1 != "0" && to_int($1) :) ))) {
	write("Bad format for site.\n");
	return;
    }
    BANISH_D->unbanish_site(parts);

    write("Done.\n");
}

