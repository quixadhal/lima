/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** save.c -- simple player save cmd
**
** 950711, Deathblade: Created.
*/

//:PLAYERCOMMAND
//USAGE:	save
//
//This command saves the present status of your character.
//You are not saved automatically until you log off
//If something happens to you, like finishing a quest, and you want to
//make sure it saves, this command will guarantee it.

inherit CMD;

private void main(string arg)
{
  this_body()->save_me();
  out("Saved.\n");
  return;
}
