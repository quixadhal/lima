/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

//:COMMAND
// Reads the filename and performs the commands specified in the file
// Useful for when needing to perform a set of commands many times
// The specification is: tsh file
// We will receive: ({ filename })
private void main( mixed *arg )
{
    map_array( explode( read_file( arg[0] ), "\n" ),
		(: this_user()->force_me($1) :) );
}

