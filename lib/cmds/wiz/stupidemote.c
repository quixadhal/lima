/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: feelings, m_messages, addemote, showemote, rmemote, targetemote
//USAGE:  stupidemote <soul>
//
//This command will add an emote with default rules of "" and "SR".
//
//stupidemote cry
//
//If you then do a showemote cry
//
//    "" -> $N $vcry.
//    "STR" -> $N $vcry $o.

#include <daemons.h>

inherit CMD;

nomask private void main(string str)
{
  if (!str)
  {
    write("stupidemote verb\n");
    return;
  }
  SOUL_D->add_emote(str, "", "$N $v"+str+".", 0);
  SOUL_D->add_emote(str, "STR", "$N $v"+str+" $o.", 0);
  out("Added.\n");
}
