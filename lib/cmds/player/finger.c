/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <mudlib.h>
#include <commands.h>

inherit CMD;

private void main( string who )
{
    resend(CMD_OB_FINGER_REAL, who);
}

void player_menu_entry(string who)
{
  bare_init();
  main(who);
  done_outputing();
}
