/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

//:COMMAND
//$$ see: vis
//USAGE: invis
//
//This command will let you turn invisible to players
//This is not intended as a "foolproof" way to be invisible.
//If you want to make sure players can't interrupt you while you work,
//either work offline or enforce a strict "do not disturb" policy.

#include <setbit.h>
#include <daemons.h>

inherit CMD;


private void main() {
    string msg;
    if(!this_body()->is_visible())
    {
	out("You are already invisible.\n");
	return;
    }
    this_body()->do_player_message("invis");
    out("You are now invisible.\n");
    this_body()->set_visibility(0);

    FINGER_D->update_me();
    return;
}


