/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz and Davmar
#include <mudlib.h>
#include <setbit.h>
#include <daemons.h>

inherit CMD;


private void main() {
    string msg;
    if (!this_body()->test_flag(INVIS))
	printf("You are not invisible.\n");
    else
    {
	this_body()->clear_flag(INVIS);
	FINGER_D->update_me();
	this_body()->do_player_message("vis");
	write("You are now visible.\n");
    }
}
