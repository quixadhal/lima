/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** verbose.c
**
*/

#include <playerflags.h>
#define USAGE "Usage: verbose [on|off]\n"

inherit CMD;



private string query_setting()
{
    return this_body()->test_flag(F_BRIEF) ? "off" : "on";
}

nomask private void main(string arg)
{
    if ( !arg || arg == "" )
    {
	write("Verbose mode is " + query_setting() +
	      ".\n" + USAGE);
	return;
    }

    switch ( arg )
    {
    case "on":
	this_body()->clear_flag(F_BRIEF);
	break;

    case "off":
	this_body()->set_flag(F_BRIEF);
	break;

    default:
	write(USAGE);
	return;
    }

    write("Verbose mode is now " + query_setting() + ".\n");
}
