/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz

#include <daemons.h>
#include <mudlib.h>

inherit CMD;



private void main( mixed arg, mapping flags )
{
    mixed tmp;

    if (flags["f"])
    {
	tmp = BANISH_D->show_banishes();
	tmp = wrap(sprintf("Banished names: %s\nBanished sites: %s\n",
	    implode( tmp[0], ", " ), implode( map_array( tmp[1],
		(: implode($1, ".") :)) , ", " )));
        clone_object(MORE_OB)->more_string(tmp);
	return;
    }

    tmp = arg[0];
    if ( !tmp ) {
        write("Missing site/name to banish\n");
	return;
    }
    
    if( strsrch(tmp,".") == -1 ) {
	BANISH_D->banish_name( tmp );
	write("Done.\n");
	return;
    }
    

    tmp = explode(tmp,".");

    if(sizeof(tmp) != 4 || sizeof(filter_array(tmp,
	(: $1 != "*" && $1 != "0" && to_int($1) :) ))) {
	
	write("Bad format for site.\n");
	return;
    }
    
    BANISH_D->banish_site( tmp );

    write("Done.\n");
    return;
}
