/* Do not remove the headers from this file! see /USAGE for more info. */

// Say
// Some cheap hack probably done in a hurry by Rust.

#include <mudlib.h>

//
inherit M_GRAMMAR;


int main(string s) {
    if (!s | s == "") {
	write("Say what?\n");
	return 0;
    }

#ifdef ZORKMUD
    this_body()->simple_action("$N $vsay, \"$O\"\n", punctuate(s));
#else
    this_body()->simple_action("$N $vsay: $o\n", punctuate(s));
#endif
    return 1;
}
