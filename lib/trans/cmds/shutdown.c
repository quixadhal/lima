/* Do not remove the headers from this file! see /USAGE for more info. */

//Earendil
#include <mudlib.h>
#include <wizlevels.h>

inherit DAEMON;


int main(string s)
{
    if ( !check_privilege(1) )
	return (int)printf("Only admins may shut down the game.\n");
    if(!s || s == "" )
	return printf("You must give a reason to shut the game down.\n"),1;
    write_file("/log/SHUTDOWN", sprintf("SHUTDOWN (%s) by %s [%s]\n",
	ctime(time()), this_body()->query_name(), s));
    shout("LPmud shut down by "+this_body()->query_name()+".\n");
    users()->quit();
    shutdown();
}
