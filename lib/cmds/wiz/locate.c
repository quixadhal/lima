/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: locate <playername>
//
//Sends out a "locate query" over intermud.
//If the player is present another mud currently connected to intermud,
//a reply is sent (subject to that mud's treatment of invisibility etc).

inherit CMD;

private void main(string* arg)
{
  IMUD_D->do_locate(arg[0]);

  out("Locate query sent.\n");
}
