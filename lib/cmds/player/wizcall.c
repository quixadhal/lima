/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;


private void main(string arg)
{
    object * wizlist;
    object wiz;

    if ( !arg )
    {
	write("Wizcall what?\n");
	return;
    }

    wizlist = filter_array(users(), (: wizardp :));
    if ( !sizeof(wizlist) )
    {
	write("You get the feeling no one hears you.\n");
	return;
    }

    foreach ( wiz in wizlist )
	tell_object(wiz, iwrap(sprintf("%s wizcalls: %s\n",
				       this_body()->query_name(), arg)));

    if ( member_array(this_user(), wizlist) == -1 )
	printf("You wizcall: %s\n", arg);
}
