/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek, End of July

#include <mudlib.h>
inherit CMD;

private void main(string arg) {
//    if(this_user()->query_userid() == "iizuka") return;
    if (!arg) {
	arg = this_body()->query_nickname();
	if( !arg )
	    out("Nickname yourself what?\n");
	else
	    out( "Your nickname is \"" + this_body()->query_nickname() + "\".\n");
	return;
    }

    arg = lower_case( arg );
    this_body()->set_nickname(arg);
    out("Ok.\n");
}

