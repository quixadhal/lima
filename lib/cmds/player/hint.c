/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE hint
//
//This brings up a menu-driven selection of hints for the quests.
//If you persevere searching through them, you might even find a useful one.
//But it's probably quicker and easier just to get on with the quest,
//unless you are really, really stuck ....

inherit CMD;

private void main()
{
  new(HINT_MENU)->start_menu();
}
