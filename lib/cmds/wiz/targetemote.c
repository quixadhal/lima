/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: feelings, m_messages, addemote, rmemote, showemote, stupidemote
//USAGE:  targetemote
//
//This command will create a default emote with the following rules;
//   LIV, OBJ, STR, ""
//
//>targetemote swim
//Added.
//
//Lets look at what was created by that command
//>showemote swim
//"LIV" -> $N $vswim at $t.
//"OBJ" -> $N $vswim at $o.
//"STR" -> $N $vswim $o.
//"" -> $N $vswim.

#include <daemons.h>

inherit CMD;

nomask private void main(string str) {
  if (!str)
  {
    out("targetemote verb\n");
    return;
  }
  SOUL_D->add_emote(str, "", "$N $v"+str+".", 0);
  SOUL_D->add_emote(str, "STR", "$N $v"+str+" $o.", 0);
  SOUL_D->add_emote(str, "OBJ", "$N $v"+str+" at $o.", 0);
  SOUL_D->add_emote(str, "LIV", "$N $v"+str+" at $t.", 0);
  out("Added.\n");
}
