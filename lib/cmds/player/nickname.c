/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek, End of July

//:PLAYERCOMMAND
//USAGE: nickname <name>
//
//The nickname command is used to give yourself a nickname.
//Other players can then use that nickname in referring to you.

inherit CMD;

private void main(string arg)
{
  if (!arg)
  {
    arg = this_body()->query_nickname();
    if( !arg )
      out("Nickname yourself what?\n");
    else
      out( "Your nickname is \"" + this_body()->query_nickname() + "\".\n");
    return;
  }

  arg = lower_case( arg );
  this_body()->set_nickname(arg);
  out("Ok.\n");
}

