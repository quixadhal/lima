/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: mvemote <target> <destination>
//
//Moves (ie renames) the specified soul to the specified destination.
//
//NOTE This is admin-only.

inherit CMD;

private void main(string str)
{
  string dest;
  if ( !adminp(this_user()) )
  {
    out("Sorry, admin only.\n");
    return;
  }

  if (!str)
  {
    out("Syntax: mvemote <target> <destination>\n");
    return;
  }

  sscanf(str, "%s %s", str, dest);

  if (SOUL_D->move_emote(str, dest))
    out("Moved.\n");
  else
    out("Failed.\n");
}

