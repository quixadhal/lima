/* Do not remove the headers from this file! see /USAGE for more info. */

// Say
// Some cheap hack probably done in a hurry by Rust.

#include <mudlib.h>

inherit CMD;
inherit M_GRAMMAR;


private void main(string s) {
    if (!s | s == "") {
	write("Say what?\n");
	return;
    }

#ifdef ZORKMUD
    this_body()->simple_action("$N $vsay, \"$O\"\n", punctuate(s));
#else
    this_body()->simple_action("$N $vsay: $o\n", punctuate(s));
#endif
}

nomask int valid_resend(string ob) {
    return ob == "/cmds/player/converse";
}
