/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>
#include <mudlib.h>
#include <daemons.h>

inherit M_ACCESS;


int fill_info( string x );
int nuke_users( string x );
string get_display( string x );

private static mapping	info =([]);

/*
** These variables are restored from the link files when we build the
** "info" mapping
*/
//private string		name;
private int		level;
private string		email;

nomask int fill_info(string);

void create()
{
    mixed *tmp;
    
    set_privilege(1);

    info = ([]);
    tmp = unguarded(1,
		    (: filter_array(map_array(get_dir( "/data/links/*" ) - ({ ".",".." }),
					      (: get_dir(sprintf("/data/links/%s/*", $1)) :) ),
				    (: pointerp :)) :));

    /* recurse two levels */
    map_array( tmp, (: map_array( $1, (: fill_info :)) :));
}

nomask int fill_info( string x )
{
    mixed * last;

    if( strlen( x ) < 0 ) 
	return 0;
    x = x[0..<3];
    if( !unguarded(1, (: restore_object, LINK_PATH(x) :)) )
	return 0;
    last = LAST_LOGIN_D->query_last(x);
    info[x] = ({ last ? last[0] : 0, level, email });
}

nomask int sort_by_level( string x, string y )
{
    if( info[x][1] == info[y][1] )
	return x > y;
    return info[x][1] < info[y][1];
}

nomask int sort_by_timeon( string x, string y )
{
    if( info[x][0] == info[y][0] )
	return x > y;
    return info[x][0] < info[y][0];
}

nomask int valid_mail( string user )
{
    string x, y;

    return info[user][2] && ( sscanf( info[user][2], "%s@%s", x, y ) == 2 );
}

nomask string get_mail( string user )
{
    return info[user][2];
}

nomask int filter_levels( string x, int min, int max )
{
    return info[x][1] >= min && info[x][1] <= max;
}

nomask int filter_times( string x, int min, int max )
{
    if ( catch(info[x][0]) ) { printf("x=%O  info[x]=%O\n",x,info[x]); return 0; }

    return info[x][0] >= min && info[x][0] <= max;
}

nomask string * parse_desire(
  int sortby,
  int minlev,
  int maxlev,
  int mintime,
  int maxtime,
  int nuke,
  string alias,
  string * except
  )
{
    string	*users;
    string	*display;
    string	x,y;

    users = keys( info );

    if( pointerp(except) ) 
	users -= map_array( except, (: lower_case :) );
    switch( sortby )
    {
    case 2:
	users = sort_array( users, (: sort_by_timeon :) );
	break;
    case 1:
	users = sort_array( users, (: sort_by_level :) );
	break;
    default:
	users = sort_array( users, 1 );
	break;
    }

    if ( maxlev < ~(1 << 31) || minlev > (1 << 31) )
	users = filter_array( users, (: filter_levels($1, $(minlev),$(maxlev)) :) );

    if( mintime || maxtime )
    {
	if( !maxtime )
	    maxtime = time();
	users = filter_array( users, (: filter_times($1, $(mintime), $(maxtime)) :) );
    }

    display = map_array( users, (: get_display :) );


    if( alias )
    {
	x = wiz_dir(this_user()) + "/.unix.mailrc";
        y = sprintf("alias %s %s\n",alias, implode( map_array(
	  filter_array( users, (: valid_mail :) ), (: get_mail :) ), "," ) );
	unguarded( 1, (: write_file, x, y :) );

	write( "Mail list written to ~/.unix.mailrc.\n" );
    }

    if( nuke )
    {
	filter_array( users, (: nuke_users :) );
	create();
    }

    return display;
}

nomask string get_display( string user )
{
    return sprintf( "%-15s%-7s%-4s%-6d%-40s", capitalize(user),
      info[user][0] ? ctime( info[user][0] )[4..9] : "NEVER",
      info[user][0] ? ctime( info[user][0] )[<2..] : "",
      info[user][1], info[user][2] ? info[user][2] : "NONE" );
}

nomask int nuke_users( string user )
{
    if ( !check_privilege(1) )
	return 0;

    if( GROUP_D->member_group( user, "admin" ) )
    {
	printf("Nuke:  Can't nuke admin: %s\n", user );
	return 0;
    }

    unguarded(1, (: rm, LINK_PATH(user)+".o" :));
    unguarded(1, (: rm, USER_PATH(user)+".o" :));

    // ### deal with mail somehow...
//    rm( "/data/mail/"+user+".o" );
    printf("Nuking %s.\n",user);
}
