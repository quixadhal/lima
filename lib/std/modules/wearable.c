/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wearable.c
**
** Mixed in to create a wearable item.  The client should also
** mix in M_GETTABLE.
**
** 07-Sep-94. Created. Rust/Belboz.
** 12-Nov-94. Updated for "modules" approach. Deathblade.
*/

#include <mudlib.h>

void hook_state(string, string, int);

private static string	wearmsg;
private static int	is_on;


void
set_wearmsg( string s )
{
  wearmsg = s;
}

string
query_wearmsg()
{
  return wearmsg;
}

void
set_is_on( int g )
{
  is_on = g;
  hook_state("extra_short", "being worn", is_on);
  hook_state("prevent_drop", "You'll have to take it off first.\n", is_on);
}

void
do_wear()
{
    set_is_on(1);
    this_body()->simple_action("$N $vwear a $o.\n", this_object());
}

void
do_remove() {
    set_is_on(0);
    this_body()->simple_action("$N $vremove $p $o.\n", this_object());
}

mixed direct_wear_obj() {
    if( is_on )
	return "But you're already wearing it!\n";
    return 1;
}

mixed direct_remove_obj() {
    if (environment() != this_body() || !is_on)
	return "But you aren't wearing it!\n";
    return 1;
}
