/* Do not remove the headers from this file! see /USAGE for more info. */

// Originally a 5 line hack by someone :)
// Modified by Beek when he successfully more'd /std/player (a directory)
// copied and edited to form moreob by --OH.

#include <mudlib.h>

inherit CMD;


private void main( string arg )
{
    object p;
    if( !arg)
    {
        out( "You must specify an object's id.\n");
        return;
    }
    p = present( lower_case(arg), environment( this_body()));
    if( !p )
    {
        out( "Nothing here by that name.\n" );
    return;
    }
    more_file( base_name( p ) + ".c" );
}

