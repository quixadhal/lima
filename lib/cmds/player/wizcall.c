/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string arg)
{
    object * wizlist;
    object wiz;

    if ( !arg )
    {
	out("Wizcall what?\n");
	return;
    }

    wizlist = filter_array(users(), (: wizardp :));
    if ( !sizeof(wizlist) )
    {
	out("You get the feeling no one hears you.\n");
	return;
    }

    foreach ( wiz in wizlist )
	tell_object(wiz, iwrap(sprintf("%s wizcalls: %s\n",
				       this_body()->query_name(), arg)));

    if ( member_array(this_user(), wizlist) == -1 )
	outf("You wizcall: %s\n", arg);
}
