/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <daemons.h>
#include <mudlib.h>

inherit CMD;



private void main( mixed arg, mapping flags )
{
    mixed tmp;
    string reason;

    if ((flags["s"] && flags["n"]) || (!flags["s"] && !flags["n"]))
    {
        write("use banish -n to banish a name, and banish -s to banish a site.\n");
	tmp = BANISH_D->show_banishes();
	tmp = wrap(sprintf("Banished names: %s\nBanished sites: %s\n",
	    implode( tmp[0], ", " ), implode( tmp[1],", " )));
        this_user()->more(tmp);
	return;
    }

    tmp = arg[0];
    if(sizeof(arg) == 2 && pointerp(arg[1]))
      {
	reason = implode(arg[1], " ");
      }
    else
      {
	reason = "(None given)";
      }
    if( flags["n"] )
      {
	BANISH_D->banish_name( tmp, reason );
	write("Name banished.\n");
	return;
      }
    

    BANISH_D->banish_site( tmp, reason );


    write("Site banished.\n");
    return;
}
