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

    tell(wizlist, this_body()->query_name() + " wizcalls: "
			+ arg + "\n", MSG_INDENT);

    if ( member_array(this_user(), wizlist) == -1 )
	outf("You wizcall: %s\n", arg);
}
