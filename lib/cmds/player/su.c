/* Do not remove the headers from this file! see /USAGE for more info. */

// _su.c   (switch user cmd)
// Rust@ZorkMUD
// Megaboz@ZorkMUD added help 5-5-94

#include <daemons.h>
#include <mudlib.h>

inherit CMD;

private void main(string arg)
{
    if (!arg)
	arg = this_user()->query_userid();

    if ( this_body() )
    {
	this_body()->save_me();  
	this_body()->save_autoload();  
    }

    this_user()->switch_user(arg);
}
