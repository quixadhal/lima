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

/* from M_GETTABLE */
void set_dropmsg(string msg);


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
  set_dropmsg(is_on ? "You'd have to remove it first.\n" : 0);
}


int
wear()
{
  if( is_on )
	return write("But you're already wearing it!\n"), -1;

  set_is_on(1);
  return 1;  // A -1 will stop the default message from printing.

}

/*
** Can't call this remove() because of the lib-dependent lfun
*/
int
remove_hook()
{
  if( !is_on )
	return write("But it's already off!\n"),-1;

  set_is_on(0);
  return 1;
}

string extra_short()
{
    return is_on ? "being worn" : 0;
}

nomask
int
prevent_drop()
{
  return is_on;
}
