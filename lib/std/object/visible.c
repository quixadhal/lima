/* Do not remove the headers from this file! see /USAGE for more info. */

#include <setbit.h>
#include <move.h>

//:FUNCTION is_visible
//return 1 if the object can be seen.
int test_flag(mixed);
void set_flag(mixed);
void clear_flag(mixed);
void set_id(string);

int
is_visible()
{
    object ob;

    if( test_flag( INVIS ))
	return 0;

    if( ( ob = environment( this_object() ) ) && 
      ( this_body() && ob == environment( this_body() ) ) )
	return 1;

    if(ob)
      return ob->is_visible() && ob->inventory_visible();

    return 1;
}

void set_visibility(int x)
{
  x ? clear_flag(INVIS) : set_flag(INVIS);
  set_id("something");
  if(this_object()->is_living())
    {
      set_id("someone");
    }
}

int get_visibility()
{
  return !test_flag(INVIS);
}
