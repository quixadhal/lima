/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <security.h>
#include <commands.h>

object query_link();

int level;

nomask void set_level(int i)
{
    if (!interactive(query_link()))
	return;
    if(!intp(i)) return;
    if(i>5 || i<0) return;
    if( previous_object() && !check_previous_privilege(1) )
	return;
    query_link()->set("level",i);
    if( !i )
    {
	tell_object(this_object(),"You have lost your wizard status.\n");
	query_link()->set("body", PLAYER);
    }
    else if( i && !level )
    {
	level = i;
	query_link()->set("body", DEV);
	tell_object(this_object(), "Su for wizard commands.\n");
	return;
    }
    level = i;
}

nomask int query_level()
{
    if( level > 5 )
	set_level( 0 );
    return level;
}

