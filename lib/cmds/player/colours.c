/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

void do_print(string which) {
    string array colours;
    
    if (which)
	colours = ({ which });
    else
	colours = this_user()->query_colours();

    if (!colours) {
	write("No colours set.\n");
	return;
    }
    
    foreach (string item in colours) {
	string what = this_user()->query_colour(item);
	if (what)
	    printf("%-20s : %%^%s%%^%s%%^RESET%%^\n",
		   lower_case(item), upper_case(item), what);
	else
	    printf("%-20s : (not set)\n", item);
    }
}

nomask private void main(string str) {
    string which, what;
    
#ifndef CONFIGURABLE_COLOUR
    write("Support not enabled.\n");
    return;
#endif

    if (!str) {
	do_print(0);
	return;
    }
    
    if (sscanf(str, "%s %s", which, what) != 2) {
	do_print(str);
	return;
    }

    if (which == "remove") {
	this_user()->remove_colour(what);
	printf("Setting for '%s' removed.\n", what);
	return;
    }
    
    this_user()->set_colour(which, what);
    printf("'%s' set to '%%^%s%%^%s%%^RESET%%^'.\n",
	   which, upper_case(which), what);
}

