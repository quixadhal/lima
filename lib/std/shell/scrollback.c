/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

object query_owner();	// in SHELL

private static string scrollback = "";
private static string last_scrollback = "";

private nomask void cmd_scrollback()
{
    if(last_scrollback=="")
    {
	write("You have no scrollback.\n");
	return;
    }
    more(last_scrollback);
}


nomask void end_scrollback()
{
    last_scrollback = scrollback;
    scrollback = "";
}

nomask void add_scrollback(string s)
{
    if ( previous_object() != query_owner() )
	error("illegal attempt at adding scrollback data\n");

    scrollback += s;
}
