/* Do not remove the headers from this file! see /USAGE for more info. */

//  _shout.c
// Mars@ZorkMUD  4-24-94

// 961216, Deathblade: noted that the only original line in this file is
//                     the header. :-)  Go Mars!

#include <mudlib.h>
inherit CMD;
inherit M_GRAMMAR;

void create()
{
  ::create();
  no_redirection();
}

private void main(string s) {
    if (!s || s == "") {
	out("Shout what?\n");
	return;
    }

    tell(users()-({this_user()}), this_body()->query_name() + " shouts: " + punctuate(s) + "\n", MSG_INDENT);
    out("You shout: " + punctuate(s) + "\n");
}

