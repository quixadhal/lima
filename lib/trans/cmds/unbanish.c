/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <daemons.h>
#include <mudlib.h>

inherit CMD;

private void main( string arg )
{
    string * parts;
    
    BANISH_D->unbanish_name( arg );
    BANISH_D->unbanish_site( arg );

    write("Did a site and name unbanish for that string...\n");
}

