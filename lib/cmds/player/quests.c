/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** quests.c
**
** Show you all the quests on the mud.
**
** Created 24-NOV-95, Rust
*/

inherit CMD; 

nomask private void main()
{
  outf("%s currently has the following quests:\n", mud_name());
  out(wrap(implode(QUEST_D->get_goals_for_quests_cmd(),"\n")));
  out("\n\nType help <questname> for more info about a quest.\n");

}
