/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main(string s)
{
    if ( !check_privilege(1) )
      {
	printf("Only admins may shut down the game.\n");
	return;
      }
    switch (s) {
    case 0:
    case "":
	write("You must give a reason to shut the game down.\n");
	return;
    case "bite me":
	write("Bite yourself!\n");
	return;
    case "asap":
        write("As soon as you provide a reason.\n");
        return;
    default:
	if (s[0..3] == "now") {
	    write("Now is not a reason.\n");
	    return;
	}
	if (strsrch(s, "fuck") != -1) {
	    write("Ask nicely.\n");
	    return;
	}
	write_file("/log/SHUTDOWN", sprintf("SHUTDOWN (%s) by %s [%s]\n",
				ctime(time()), this_body()->query_name(), s));
	shout("LPmud shut down by "+this_body()->query_name()+".\n");
	users()->quit();
	shutdown();
    }
}


