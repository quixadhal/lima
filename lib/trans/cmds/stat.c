/* Do not remove the headers from this file! see /USAGE for more info. */

//_stat.c   (wizard command)
// Beek@ZorkMUD

// This is very OO.  Use it correctly or die. :)
// Basically you can override stat_me() using inheritance.
// 
// stat_me() {
//  ::stat_me();
//  if (this_user()->query_real_name()=="beek") {
//    write("Debug info: i = "+i);
//  }
// }

#include <mudlib.h>

inherit DAEMON;

int main(mixed *arg) {
    if (!arg[0]->stat_me()) {
	printf("No information available for %O.\n", arg[0]);
	return 1;
    }
    return 1;
}

int help() {
    write("Syntax: stat <object>\n\nGives you vital information about ANY object.\n");
    return 1;
}
