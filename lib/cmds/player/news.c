/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE:	news
//
//news starts up the newsreader.  Just type it, it has it's own internal
//help available, if you type a ? from any prompt.

inherit CMD;

private void main(string arg)
{
  new(NEWSREADER)->begin_reading(arg);
}

void player_menu_entry()
{
  bare_init();
  main("");
  done_outputing();
}