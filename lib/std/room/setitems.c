/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** setitems.c -- temporary patch to upgrade old set_items() calls
**
** 951223, Deathblade: created
*/

varargs void add_item();

#include <security.h>
inherit M_ACCESS;
void set_items(mapping m)
{                  
    string fname = file_name(this_object()) + ".c";
    string str;
    int len;
    int start;

    write("*** WARNING: set_items() usage found in "+fname+"\n");

    str = "\n\n/*\n";

    foreach ( mixed key, string value in m )
    {
	if ( pointerp(key) )
	{
	    str += ("    add_item(" +
		    implode(key, (: $1 + "\"" + $2 + "\", " :), "") +
		    "\"" + value + "\");\n"
		    );
	    add_item(key..., value);
	}
	else
	{
	    str += "    add_item(\"" + key + "\", \"" + value + "\");\n";
	    add_item(key, value);
	}
    }

#define MARKER "\n[AUTO CONVERT MARKER]\n*/\n"
    len = strlen(MARKER);

    str += MARKER;

    start = file_size(fname) - len;
    if ( start > 0 )
    {
	string s;

	s = read_bytes(fname, start, len);
	if ( s != MARKER )
	{
	    write("*** converted code appended to the file (as a comment)\n");
	    unguarded(1, (: write_file, fname, str :));
	}
	else
	{
	    write("*** converted code already exists in the file\n");
	}
    }
    else
    {
	int idx = strsrch(fname, "/", -1);

	fname = "/tmp/" + fname[idx+1..];
	write("*** funny looking file! converted code written to "+fname+"\n");
	write_file(fname, str, 1);
    }
}
