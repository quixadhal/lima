/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

string internal_long, internal_short;


void
set_internal_long( string s )
{
    internal_long = s;
}

public
string
query_internal_long()
{
    return internal_long;
}

void
set_internal_short( string s )
{
    internal_short = s;
}

public
string
query_internal_short()
{
    return internal_short;
}

public
string
short()
{
    if( environment( this_body() ) == this_object() )
	return internal_short;

    return shitty_hack_to_get_object_short();
}
public
string
long()
{
    if( environment( this_body() ) == this_object() )
	return internal_long + show_objects();

    return shitty_hack_to_get_object_long();
}

public
int
announce_board()
{
    this_body()->simple_action("$N $venter "+the_short()+".\n");
    force_look();
    tell_room( this_body()->query_last_location(),
	this_body()->query_name()+" enters "+the_short()+".\n" );
    return 1;
}

public
int
announce_failed_board()
{
    this_body()->simple_action("$N $vtry to enter "+the_short()+", but $vfail.\n");
    return 1;
}

public
int
announce_failed_exit()
{
    this_body()->simple_action("$N $vtry to exit "+the_short()+", but $vfail.\n");
    return 1;
}

public
int
announce_exit()
{
    this_body()->simple_action("$N $vleave "+the_short()+".\n");
    force_look();
    tell_room( this_body()->query_last_location(),
	this_body()->query_name()+" leaves "+the_short()+".\n" );
    return 1;
}

public
int
board()
{
    if( !this_body()->move( this_object(), query_prep() ) )
	return announce_board();

    else
	return announce_failed_board();
}

public
int
exit()
{
    if( !this_body()->move( environment( this_object() ), "in" )  )
	return announce_exit();

    else
	return announce_failed_exit();
}

int
get()
{
    return 0;
}

int
announce_go( string dir )
{
    this_body()->simple_action("$N $Vgo "+dir+" the "+short()+"." );
  force_look();
    tell_room( this_body()->query_last_location(),
	this_body()->query_name()+" leaves "+the_short()+".\n" );
    return 1;
}

int
go( string dir )
{
    if( dir != query_prep() )
	return notify_fail( "That doesn't seem to be possible.\n");

    if( !this_body()->move( this_object(),query_prep() ) )
	return announce_go( dir );

    notify_fail("That doesn't seem to be possible.\n");
    return 0;
}

/*      VERB INTERACTION     */

mixed direct_enter_obj()
{
    return 1;
}
