/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz@ZorkMUD wrote it
// Megaboz added header and help 5-5-94
// Beek decided to edit this one when he noticed we had two
// inventory commands.  *boggle*

//:PLAYERCOMMAND
//USAGE:  i
//        inventory
//
//Shows you what you have in your inventory.

inherit CMD;

private void main()
{
  string ex = inv_list(all_inventory(this_body()));

  if (ex)
    out("You are carrying:\n" + ex);
  else
    out("You are empty handed.\n");

  return;
}
