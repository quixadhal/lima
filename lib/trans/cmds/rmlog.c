/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;

// Rust
int main()
{
  string	file;

  file = wiz_dir(this_user()) + "/log";

  if(file_size(file) <1)
	return write("No log found.\n"), 1;

  rm( file );
  return write("Error log removed.\n"), 1;
}
