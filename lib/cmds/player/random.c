/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** random.c -- invoke a random emote
**
** 950826, Deathblade: created.
*/

//:PLAYERCOMMAND
//$$ see: feelings, adverbs, semote
//
//USAGE: random
//
//This command will randomly select a feeling to execute.

#include <daemons.h>

inherit CMD;

private void main(string str)
{
  string * emotes = SOUL_D->list_emotes();
  string name = lower_case(trim_spaces(str));
  string rule;
  string extra;

  if ( find_body(name) )
  {
    rule = "LIV";
    extra = " " + name;
  } else {
    rule = extra = "";
  }

  while ( 1 )
  {
    string emote = choice(emotes);

    if ( SOUL_D->query_emote(emote)[rule] )
    {
      this_body()->do_game_command(emote + extra);
      return;
    }
  }
}