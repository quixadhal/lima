/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <mudlib.h>
#include <setbit.h>
#include <daemons.h>




int main() {
    string msg;
    if(this_body()->query_flag(INVIS))
    {
	write("You are already invisible.\n");
	return 1;
    }
    this_body()->do_player_message("invis");
    write("You are now invisible.\n");
    this_body()->set_flag(INVIS);

    FINGER_D->update_me();
    return 1;
}


