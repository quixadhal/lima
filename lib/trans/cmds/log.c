/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

// Belboz
private void main()
{
  string	file;

  file = wiz_dir(this_user()) + "/log";

  if(file_size(file) <1)
      write("No log.\n");
  else
      tail(file);
}

int help(){
return write("Usage: log\nTails your error log.\n"), 1;
}
