/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit M_PARSING;

void interrogate( string verb )
{
    string noun;
    write( "What do you want to " + verb + "?\n" );
}
