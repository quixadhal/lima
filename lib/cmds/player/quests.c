/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** quests.c
**
** Show you all the quests on the mud.
**
** Created 24-NOV-95, Rust
*/

//:PLAYERCOMMAND
//USAGE quests
//
//Shows you all the quests on the mud.

inherit CMD;

string modify(string quest)
{
  string quests = this_body()->query_quests();
  if(!sizeof(quests) || member_array(quest, quests)<0)
    return quest;
  else
    return quest + "*";
}

nomask private void main()
{
  outf("%s currently has the following quests:\n", mud_name());
  out("(* after the quest indicates you have completed it)\n");
  out(implode(map(QUEST_D->get_goals_for_quests_cmd(),(:modify:)),"\n"));
  out("\n\nType help <questname> for more info about a quest.\n");
  out ("Or use the hint system.\n");
}
