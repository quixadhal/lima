/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust Aug 15 cause Peregrins finger_d is badly in need of re-working
// due to changes and I want something working to finish the gwiz package.

#include <security.h>
#include <mudlib.h>
#include <levels.h>

inherit M_ACCESS;

string show_big_finger();

string 	name;
string  email;
string  real_name;
int 	level;
mapping mailbox_flags;


mixed * get_raw_data(string who)
{
    string mailstring;
    string linkpath, userpath;
    int i;
    string retval;
    mixed * last;

    real_name = email = mailbox_flags = 0;

    linkpath = LINK_PATH( who );
    if( unguarded( 1, (: file_size($(linkpath + ".o")) :) ) <= 0 )
	return 0;

    unguarded( 1, (: restore_object, linkpath :) );

    /* load some user info... make sure to NOT zero out vars */
    userpath = USER_PATH( who );
    unguarded( 1, (: restore_object, userpath, 1 :) );

//    if( file_size( "/wiz/"+name+"/.plan" ) > 0 )
//	retval += "\n" + read_file( "/wiz/"+name+"/.plan" );

    last = LAST_LOGIN_D->query_last(who);
    return ({ capitalize(name),
		  LEVELS[level],
		  real_name,
		  email,
		  last ? ctime(last[0]) : "<unknown>",
		  0,
		  last ? last[1] : "<unknown>",
		  0,
		  0
		  });
}

string
get_finger( string who )
{
    string mailstring;
    string linkpath, userpath;
    int i;
    string retval;
    mixed last;

    real_name = email = mailbox_flags = 0;
    if( !( stringp( who ) && strlen( who ) ) )
	return (string)show_big_finger();
    linkpath = LINK_PATH( who );
    if( unguarded( 1, (: file_size($(linkpath + ".o")) :) ) > 0 )
    {
	unguarded( 1, (: restore_object, linkpath :) );

	/* load some user info... make sure to NOT zero out vars */
	userpath = USER_PATH( who );
	unguarded( 1, (: restore_object, userpath, 1 :) );

	if( this_user() &&
	   ( GROUP_D->adminp(this_user()) ||
	    this_user()->query_real_name() == name) )
	{
	    email = email || "None given";
	    real_name = real_name || "None given";
	}
	else
	{
	    email = ( email && ( email[0] == '#' ? "Private" : email ) ) ||
		"None given";
	    real_name = ( real_name && (real_name[0] == '#' ? "Private" : real_name ) )
		|| "None given";
	}
	if( !mapp( mailbox_flags ) || !i = sizeof( mailbox_flags ) )
	    mailstring = capitalize( name )+" has no mail.\n";

	else
	{
	    mailstring = sprintf("%s has %d messages", capitalize(name),i);
	    if ( i = sizeof( filter_array( keys( mailbox_flags ),
					  (: mailbox_flags[$1] :) ) ) )	  
		mailstring += sprintf( ", %d of which %s unread.",
				      i, ( i > 1 ? "are" : "is" ) );
	    else
		mailstring += ".";
	}

	last = LAST_LOGIN_D->query_last(who);
	retval = sprintf("%s\nIn real life: %-36sLevel: %s\n"
			 "%s %s from %s\n%s\nEmail Address: %s\n",
			 capitalize( name ),
			 real_name,
			 LEVELS[level],
			 find_body( name ) ? "On since" : "Last on",
			 last ? ctime(last[0]) : "<unknown>",
			 last ? last[1] : "<unknown>",
			 mailstring,
			 email);

	if( file_size( "/wiz/"+name+"/.plan" ) > 0 )
	    retval += "\n" + read_file( "/wiz/"+name+"/.plan" );

	return retval;
    }
    return "No such player.\n";
}

static
string
get_level( object o )
{
  return LEVELS[ (o->query_body()->query_level()) ];
}

static
string
get_idle( object o )
{
  int i;

  i = query_idle( o );
  if( i > 3600 )
    return ( i/3600 ) + "h";
  if( i > 60 )
    return ( i/60 ) + "m";

  return "";
}

string
show_big_finger()
{
  mixed crap;
  int	i;

  crap = map_array(users(),
		   (: sprintf("%-15s%-12s%-5s%s",
			      capitalize($1->query_real_name()),
			      get_level($1),
			      get_idle($1),
			      query_ip_name($1)) :));

  crap = implode( crap, "\n" ) + "\n";
  crap = sprintf("\n[%s] %d user%s presently connected (%s)\n%79'-'s\n",
	mud_name(),
	i = sizeof( users() ),
	( i == 1 ? "" : "s" ),
	ctime(time()),
	"" ) + crap;
  return crap;
}
