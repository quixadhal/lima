/* Do not remove the headers from this file! see /USAGE for more info. */

//_stat.c   (wizard command)
// Beek@ZorkMUD

// This is very OO.  Use it correctly or die. :)
// Basically you can override stat_me() using inheritance.
// 
// stat_me() {
//  ::stat_me();
//  if (this_user()->query_userid()=="beek") {
//    write("Debug info: i = "+i);
//  }
// }

#include <mudlib.h>
inherit CMD;

private void main(mixed *arg) {
    if (!arg[0]->stat_me()) {
	outf("No information available for %O.\n", arg[0]);
	return;
    }
    return;
}

int help() {
    write("Syntax: stat <object>\n\nGives you vital information about ANY object.\n");
}
