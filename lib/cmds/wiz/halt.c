/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: halt
//
//Stops all combat in the room

inherit CMD;

private void main()
{
  all_inventory(environment(this_body()))->stop_fight();
  this_body()->simple_action("$N $vhalt combat in the room.");
}
