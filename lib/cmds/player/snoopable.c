/* Do not remove the headers from this file! see /USAGE for more info. */

// _snoopable.c
// this space intentionally left blank
// Megaboz@ZorkMUD added header and help

//:PLAYERCOMMAND
//$$ see: privacy
//USAGE:  snoopable [on|off]
//
//This does exactly what the name imples -- it sets whether you are snoopable.

#include <playerflags.h>

inherit CMD;

private void main(string arg)
{
  if ( !arg || arg == "" )
  {
    if ( this_body()->test_flag(F_SNOOPABLE) )
      out("Joe Wizard can snoop you.\n");
    else
      out("Joe wizard can't snoop you.\n");
    return;
  }

  switch ( arg )
  {
    case "on":
      this_body()->set_flag(F_SNOOPABLE);
      break;

    case "off":
      this_body()->clear_flag(F_SNOOPABLE);
      break;

    default:
      out("Syntax: snoopable (on|off)\n");
      return;
  }

  out("Ok.\n");
}

void player_menu_entry(string str)
{
  bare_init();
  main(str);
  done_outputing();
}