/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

// Rust

private void main(mixed *arg) {
  string file;
  int i;
  string chunk;

  foreach(file in arg[0])
    {
      i = 0;
      if(!is_file(file))
	printf("Warning: %s failed.\n", file);
      else
	chunk = read_file(file);
//	while(chunk = read_file(file,i,i+=100))  crashes the driver on large files
	  write(chunk);
    }
    return;
}


int help()
{
    write(wrap(
	"Usage:  cat <filename>\n"
	"Displays the entire contents of the specified file to your screen "
	"all at once.  See also more.\n" ));
}
