/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE: help or help <topic>
//
//With no arguments, you are sent into the general help menu.  You may instead, pass a topic as an argument.
//
//Within the help system, when you are at the %^BOLD%^Help topic? [?]:%^RESET%^ prompt, you can enter another topic for //help on that.  Do not enter 'help' as you do from the commandline.  Just the topic name.
//
//If there are more than one topic with the same name that you select, you will be given the option of selecting which //match you wish to read.
//
//Help files are paged through the MUD's standard 'more' pager.  For help on that please refer to the help page for more, //or type ? or h at the more prompt.

inherit CMD;

private void main(string arg){ new(HELPSYS)->begin_help(arg); }

void player_menu_entry()
{
  bare_init();
  main("");
  done_outputing();
}
