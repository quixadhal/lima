/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;
int main(mixed *arg) {
  string dir;
  foreach(dir in arg[0])
    {
      if (rmdir(dir))
	printf("Removed directory: %s.\n", dir);
      else
	printf("Failed to remove directory: %s.\n", dir);
    }
 
}
