/* Do not remove the headers from this file! see /USAGE for more info. */

//  _shout.c
// Mars@ZorkMUD  4-24-94

#include <mudlib.h>


inherit M_GRAMMAR;


int main(string s) {
    if (!s | s == "") {
	write("Shout what?\n");
	return 0;
    }

map(users()-({this_user()}), (: tell_object($1, iwrap(sprintf("%s shouts: %s\n", this_body()->query_name(), punctuate($2)))) :), s);
    write(iwrap("You shout: " + punctuate(s) + "\n"));
    return 1;
}
