/* Do not remove the headers from this file! see /USAGE for more info. */

// specification is: snoop [user] [str*]
// we will receive: ({ user, ({ words }) })

//:COMMAND
//$$see: unsnoop, snoops
//USAGE:  snoop < name >
//
//This command will allow you to see exactly what appears on the
//named characters screen, if the player has snoopable on.
//Admins can snoop anybody at any time.
//
//ALL SNOOPS ARE LOGGED.
//Spying on a player for no better reason than just to do it will
//not be tolerated by the admin of this mud.
//Use this command to help find bugs and see what exactly the player
//did etc.  but in no way use it to spy on players.


#include <log.h>

inherit CMD;

private void main(mixed *arg)
{
  int rc;
  if ( !arg[0] )
  {
    out("Try: unsnoop [player] instead.\n");
    return;
  } else {
    string msg;

    if ( arg[1] )
      msg = implode(arg[1], " ");
    else
      msg = "no reason given";

    msg = sprintf("%s snooped %s because: %s\n",
          this_user()->query_userid(),
          arg[0]->query_link()->query_userid(),
          msg);

    rc = SNOOP_D->snoop(this_body(), arg[0]);
    switch(rc)
    {
      case 0:
        LOG_D->log(LOG_SNOOP, msg);
        out("Okay.\n");
        return;
      case -1:
        out("Uhhh... you're already snooping that one, holmes.\n");
        return;
      case -2:
        out("Doh! That would make a snoop cycle! No can do...\n");
        return;
      case -3:
        out("No permissions, sorry...\n");
        return;
      default:
        out("You can't, but I can't tell you why...\n");
        return;
    }
  }
}