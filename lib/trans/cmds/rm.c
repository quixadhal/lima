/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <mudlib.h>

inherit CMD;

private void main(mixed argv)
{
  map(argv[0], (: rm($1) ? outf("%s: removed.\n", $1) : 
		outf("failed to remove: %s\n", $1) :));
}
