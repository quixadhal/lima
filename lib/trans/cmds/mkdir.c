/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main(string* arg) {
  if (mkdir(arg[0]))
     printf("Ok.\n");
  else
     printf("Failed to make directory: %s\n", arg[0]);
}
