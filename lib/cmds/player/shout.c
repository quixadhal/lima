/* Do not remove the headers from this file! see /USAGE for more info. */

//  _shout.c
// Mars@ZorkMUD  4-24-94

#include <mudlib.h>
inherit CMD;
inherit M_GRAMMAR;

void create()
{
  ::create();
  no_redirection();
}

private void main(string s) {
    if (!s | s == "") {
	out("Shout what?\n");
	return;
    }

    map(users()-({this_user()}), (: tell_object($1, iwrap(sprintf("%s shouts: %s\n", this_body()->query_name(), punctuate($2)))) :), s);
    out(iwrap("You shout: " + punctuate(s) + "\n"));
}

