/* Do not remove the headers from this file! see /USAGE for more info. */

// The specification is: tail file *
// We will recieve: ({ array of file names })

#include <mudlib.h>
inherit CMD;

private void main( mixed *arg ) {
  int i;
  for (i=0; i<sizeof(arg[0]); i++) {
    tail(arg[0][i]);
  }
}

int help() {
  printf("Usage: tail [file]\n"
    "Description: display the last lines of a file\n");
  return 1;
}
