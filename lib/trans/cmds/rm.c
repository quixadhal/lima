/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <mudlib.h>

inherit DAEMON;

int main(mixed argv)
{
  map(argv[0], (: rm($1) ? printf("%s: removed.\n", $1) : 
		printf("failed to remove: %s\n", $1) :));
}
