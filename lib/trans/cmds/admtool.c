/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove the headers from this file! see /USAGE for more info. */  

#include <mudlib.h>

inherit CMD;

private void main(string arg)
{
    if (arg == "-n")
	new("/obj/admtool/admtool2");
    else if (arg)
	write("Unknown option.\n");
    else
	new(ADMTOOL);
}
