/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** biff.c
**
** Adjust the "biff" setting (mail announcements)
**
** Created 31-OCT-94, Deathblade.
*/

//:PLAYERCOMMAND
//USAGE biff
//      biff on|off
//
// Adjust the "biff" setting (mail announcements)

#include <flags.h>

inherit CMD;

private string query_setting()
{
  return this_body()->test_flag(F_BIFF) ? "on" : "off";
}

nomask private void main(string arg)
{
  if ( !arg || arg == "" )
  {
    out("Your biff mail notification flag is " + query_setting() + ".\n");
    return;
  }

  switch ( arg )
  {
    case "on":
      this_body()->set_flag(F_BIFF);
      break;

    case "off":
      this_body()->clear_flag(F_BIFF);
      break;

    default:
      out("Usage: biff [on | off]\n");
      return;
  }

  out("Your biff mail notification flag is now " + query_setting() + "\n");
}

void player_menu_entry(string str)
{
    bare_init();
    main(str);
    done_outputing();
}
