/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

// The specification is: tsh file
// We will receive: ({ filename })
private void main( mixed *arg )
{
    map_array( explode( read_file( arg[0] ), "\n" ),
		(: this_user()->force_me($1) :) );
}

