/* Do not remove the headers from this file! see /USAGE for more info. */

#include <setbit.h>
#include <move.h>

//:FUNCTION is_visible
//return 1 if the object can be seen.

//### who uses this?
int
is_visible()
{
    object ob;

    if( this_object()->test_flag( INVIS ) )
	return 0;

    if( ( ob = environment( this_object() ) ) && 
      ( ob == environment( this_body() ) ) )
	return 1;

    return ob->is_visible() && ob->inventory_visible();
}
