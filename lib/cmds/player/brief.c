/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** brief.c
**
*/

#include <playerflags.h>
#define USAGE "Usage: brief [on|off]\n"

inherit CMD;



private string query_setting()
{
    return this_body()->test_flag(F_BRIEF) ? "on" : "off";
}

nomask private void main(string arg)
{
    if ( !arg || arg == "" )
    {
	out("Brief mode is " + query_setting() +
	      ".\n" + USAGE);
	return;
    }

    switch ( arg )
    {
    case "on":
	this_body()->set_flag(F_BRIEF);
	break;

    case "off":
	this_body()->clear_flag(F_BRIEF);
	break;

    default:
	out(USAGE);
	return;
    }

    out("Brief mode is now " + query_setting() + ".\n");
}
