/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

// Rust
private void main()
{
  string	file;

  file = wiz_dir(this_user()) + "/log";

  if(file_size(file) <1)
      write("No log found.\n");
  else {
      rm( file );
      write("Error log removed.\n"), 1;
  }
}
