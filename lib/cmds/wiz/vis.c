/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust
#include <mudlib.h>
#include <setbit.h>
#include <daemons.h>

inherit CMD;


private void main() 
{
    string msg;
    if (this_body()->is_visible())
	printf("You are not invisible.\n");
    else
    {
	this_body()->set_visibility(1);
	FINGER_D->update_me();
	this_body()->do_player_message("vis");
	write("You are now visible.\n");
    }
}
