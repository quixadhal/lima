/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

//:PLAYERCOMMAND
//USAGE: finger  or
//       finger player  or
//       finger @mudname  or
//       finger player@mudname
//
//Gives you information about a player named.  If you do not mention a
//particular mud, it searches for that player info here.  If you do not
//mention a player or a mud, it gives general finger info for everyone
//on line here.  If you mention another mud but no player, it gives you
//general info on the players on that mud.
//
//The mud name doesn't have to be the complete name, and can be a
//partial match, as long as you provide enough information so that the
//game can figure out which mud you're talking about.

#include <commands.h>

inherit CMD;

private void main( string who ){ resend(CMD_OB_FINGER_REAL, who); }

void player_menu_entry(string who)
{
  bare_init();
  main(who);
  done_outputing();
}
