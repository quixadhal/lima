/* Do not remove the headers from this file! see /USAGE for more info. */

// specification is: snoop [user] [str*]
// we will recieve: ({ user, ({ words }) })
#include <mudlib.h>
#define  ASNOOP_LOG  "/log/snoops"

inherit DAEMON;
int main(mixed *arg) {
    if(!arg[0])
	snoop();
    else {
	if (!arg[1])
	    snoop(arg[0]->query_link());
	else {
	  write_file(ASNOOP_LOG, sprintf("%s snooped %s because: %s\n",
					 this_user()->query_userid(),
					 arg[0]->query_link()->query_userid(),
					 implode(arg[1]," ")));
	  unguarded(1, (: snoop($(arg[0])->query_link()) :));
	}
    }
    return 1;
}
