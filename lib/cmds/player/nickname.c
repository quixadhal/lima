/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek, End of July

#include <mudlib.h>
inherit CMD;

private void main(string arg) {
    if (!arg) {
	out("Nickname yourself what?\n");
	return;
    }
    
    this_body()->set_nickname(arg);
    out("Ok.\n");
}

