/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz@ZorkMUD wrote it
// Megaboz added header and help 5-5-94
// Beek decided to edit this one when he noticed we had two
// inventory commands.  *boggle*

#include <mudlib.h>
inherit CMD;


private void main()
{
    string ex;

    if (ex = inv_list(all_inventory(this_body()))) {
        write("You are carrying:\n" + ex);
    } else {
	write("You are empty handed.\n");
    }
    return;
}
