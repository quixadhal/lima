/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: tsh <filename>
//
// Reads the filename and performs the commands specified in the file
// Useful for when needing to perform a set of commands many times

inherit CMD;

private void main( mixed *arg )
{
  map_array( explode( read_file( arg[0] ), "\n" ),
      (: this_user()->force_me($1) :) );
}

