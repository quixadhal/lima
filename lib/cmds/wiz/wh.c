/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;



private void main( mixed * arg )
{
    array targets;
    string str;

    outf("%-13s%-30s%-s\n","Who","File","Room");
    out("---------------------------------------------------------------\n");
    if( !arg[0])
	targets = users()->query_userid();
    else
    {
	str = implode(arg[0], ",");
	targets = explode( str, ",") - ({ });
    }
    foreach( string target in targets )
    {
	target = trim_spaces(target);
	if( !user_exists( target ))
	{
	    outf("%-13s%-30s[Only in your Imagination]", capitalize( lower_case( target )), "Does not exist " );
	}
	else if( member_array( target, users()) != -1)
	{
	    outf("%-13s%-30s[%-s]\n", capitalize( find_body(target)->query_userid()), 		      file_name(environment(find_body(target))), environment(find_body(target))->short());
	}
	else
	{
	    outf("%-13s%-30s[%-s]\n", capitalize(
		lower_case( target)), "Not Currently On ", "Physical Storage");
	}
    }
}


