/* Do not remove the headers from this file! see /USAGE for more info. */

// Taken off of psycho
// Megaboz@ZorkMUD attached header

//:PLAYERCOMMAND
//You don't understand what quit does??

inherit CMD;

private void main()
{
  this_user()->quit();
}