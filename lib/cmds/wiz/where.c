/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;


private void main( mixed * arg )
{
    string array targets;
    string array bodies = bodies()->query_userid();
    object curr_body;

    outf("%-13s%-30s%-s\n","Who","File","Room");
    out("---------------------------------------------------------------\n");
    if( !arg[0])
	targets = bodies;
    else
    {
    targets = explode( implode( arg[0], "," ), ",") - ({ }) - ({ "" });
    }
    foreach( string target in targets )
    {
	target = trim_spaces(target);
	if( !user_exists( target ))
	{
	    outf("%-13s%-30s[Only in your Imagination]\n", capitalize( lower_case( target )), "Does not exist " );
	}
	else if (member_array( target, bodies ) != -1 )
	{
	    curr_body = find_body( target );
	    outf( "%-13s%-30s[%-s]\n", capitalize( curr_body->query_userid()), file_name(environment(curr_body)), environment(curr_body)->short());
	}
	else
	{
	    outf("%-13s%-30s[%-s]\n", capitalize(
		lower_case( target)), "Not Currently On ", "Physical Storage");
	}
    }
}


