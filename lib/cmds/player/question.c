/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust wrote it
// Megaboz@ZorkMUD attached header 5-5-94
// Converted to use new /std/reporter superclass (Deathblade 4-Sep-94)
// Beek made one line reports work right 11/14/94
// Rust made them post news

//:PLAYERCOMMAND
//$$ see: bug, typo, feedback, idea
//USAGE:  question
//
//Allows you to send a question to the relevant place.

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string str)
{
  REPORTER_D->report_something("Question", str);
}

void player_menu_entry()
{
  bare_init();
  main("");
  done_outputing();
}
