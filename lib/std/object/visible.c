/* Do not remove the headers from this file! see /USAGE for more info. */

#include <setbit.h>
#include <move.h>

//:FUNCTION is_visible
//return 1 if the object can be seen.
int test_flag(mixed);
void set_flag(mixed);
void clear_flag(mixed);
void set_id(string);
void remove_id(string array ...);

void resync_visibility() {
    // The parser needs to know whether our contents can be seen
    parse_refresh();
}

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

string invis_name() {
    return "something";
}

void set_visibility(int x)
{
    if (x) {
	clear_flag(INVIS);
    } else {
	set_flag(INVIS);
    }
    resync_visibility();
}

int get_visibility()
{
  return !test_flag(INVIS);
}
