/* Do not remove the headers from this file! see /USAGE for more info. */

// More Crypic #@$t from Belboz
//

#include <mudlib.h>




#define SYNTAX "Usage: groups [-a | -d] [groupname] [name 1] [name 2] ... \n"

private string banner =
"==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==\n";

private nomask void print_groups(mapping groups);


int main( string arg )
{
    mapping	groups;
    string	this_group;
    mixed	valid;
    mixed	not;
    string *	arglist;


    groups = this_body()->query_perm( "groups" );
    if(!groups)  
	groups = ([]);

    if( !arg || arg == "" )
    {
	write( banner );
	printf( "Groups of %s, and their members:\n\n",mud_name() );
	print_groups(GROUP_D->get_group_data());
	write( banner );


	if( !mapp( groups ) || !sizeof( groups ) )
	    return write( "You have no groups defined.\n" ), 1;

	printf( "%s, your personal groups are:\n\n", 
	       this_body()->query_name() );

	print_groups(groups);

	write(banner);
	return 1;
    }

    arglist = explode( arg, " " );

    if( sizeof(arglist) <2 || (arglist[0] != "-a" && arglist[0] != "-d") )
	return write( SYNTAX ), 1;

    this_group = arglist[1];

    if( arglist[0] == "-a" )
    {
	if( sizeof(arglist) <3 )
	    return write(SYNTAX), 1;

	arglist = filter_array(arglist[2..],
			   function(string x, string this_group, string *grp_members)
			   {
			       if ( pointerp(grp_members) &&
				   member_array(grp_members, x) != -1 )
			       {
				   printf("%s is already in group %s.\n",
					  x, this_group);
				   return 0;
			       }
			       else
			       {
				   printf("%s added to group %s.\n",
					  x, this_group);
				   return 1;
			       }
			   },
			   this_group);

	arglist = clean_array( arglist );

	this_body()->set_perm( "groups", groups +
				([ this_group : arglist ? (mixed)arglist : (mixed)(([]) + groups[this_group]) ]) );
	return 1;
    }

    if( sizeof(arglist) == 2 )
    {
	if ( member_array(groups,arglist[1]) )
	{
	    printf("Group %s nuked.\n", arglist[1]);
	    this_body()->set_perm( "groups", map_delete( groups, arglist[1] ) );
	    return 1;
	}
	printf("You have no defined group called '%s'!\n", arglist[1]);
	return 1;
    }
    arglist = arglist[2..];
    valid = filter_array(arglist,
			 (: member_array($(groups[this_group]), $1) != -1 :));

    not = clean_array( arglist - valid );
    valid = clean_array( valid );

    if( sizeof( not ) )
	write(iwrap(sprintf("Unable to remove: %s.\n", implode( not, ", "))));

    if( sizeof( valid ) )
    {
	write(iwrap(sprintf("Removed: %s.\n", implode( valid, ", " ))));
	groups[this_group] -= valid;
	if( !sizeof( groups[this_group] ) )
	    map_delete(groups, this_group);
	this_body()->set_perm( "groups", groups );
    }
    return 1;
}


private nomask void print_groups(mapping groups)
{
    map_array(sort_array(keys(groups), 1),
	      (: write(iwrap(sprintf("%s: %s\n", $1,
				     implode(map_array(sort_array($2[$1], 1),
						       (: capitalize :)), ", ")))) :),
	      groups);

    write("\n");
}

