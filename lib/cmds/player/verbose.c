/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** verbose.c
**
*/

//:PLAYERCOMMAND
//$$ see: brief
//USAGE verbose
//      verbose on|off
//
//This shows whether you are using verbose or brief mode, and allows you to
//switch between them.
//
//Room descriptions are suppressed in "brief" mode, so beware of unexpected
//encounters if you choose to use it...

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
	out("Verbose mode is " + query_setting() +
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
	out(USAGE);
	return;
    }

    out("Verbose mode is now " + query_setting() + ".\n");
}
