/* Do not remove the headers from this file! see /USAGE for more info. */

// This command works only for admins.  -- Rust

#include <mudlib.h>
inherit CMD;

private void main()
{
    int flag;
    object user;

    flag = 0;

    foreach ( user in users() )
    {
	object ob = query_snooping(user);

	if ( ob )
	{
	    outf("%s is snooping %s.\n", 
		   capitalize(user->query_userid()),
		   capitalize(ob->query_userid()));
	    flag = 1;
	}
    }

    if ( flag == 0 )
	out("Nobody is being snooped.\n");
}
