/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** brief.c
**
*/

//:PLAYERCOMMAND
//$$ see : verbose
//USAGE brief
//      brief on/off
//
//Shows or sets whether you are using "brief" mode,
//which ignores the room descriptions.
//Use "brief" mode at your own risk - it greatly increases the chance
//of missing important clues in the room descriptions,
//and of upsetting creators whose masterpieces of text you are now ignoring.

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
    out("Brief mode is " + query_setting() + ".\n" + USAGE);
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
