/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE:	menu
//
//Just type "menu" to get a menu of common commands.  Some things like
//changing your title, real name information, and finger information,
//can only be done through the menu.

inherit CMD;

private void main()
{
  new(PLAYER_MENU)->start_menu();
}
