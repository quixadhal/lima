/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE semote <feeling>
//
//Shows you how a "soul" or "feeling" will look, both to you and to target
//and onlookers, if you use it.

#include <commands.h>

inherit CMD;
inherit M_GRAMMAR;

private void main(string which)
{
  mapping data;
  mixed * info;

  if ( !which )
  {
    out("You need to specify a soul.\n");
    return;
  }

  data = SOUL_D->query_emote(which);
  if ( !data )
  {
    out("That soul does not exist!\n");
    return;
  }

  if ( data[""] )
  {
    info = SOUL_D->get_soul(which, "", ({ }));
    outf("%s :\n     You'll see: %sOthers will see: %s\n",
      which, info[1][0], info[1][1]);
  }

  if ( data["LIV"] )
  {
    info = SOUL_D->get_soul(which, "LIV", ({ this_object() }));
    outf("%s somebody :\n     You'll see: %sTarget will see: %sOthers will see: %s\n",
      which, info[1][0], info[1][1], info[1][2]);
  }

  if ( data["STR"] )
  {
    info = SOUL_D->get_soul(which, "STR", ({ "______" }));
    outf("%s ______ :\n     You'll see: %sOthers will see: %s\n",
      which, info[1][0], info[1][1]);
  }

  if ( data["LIV STR"] )
  {
    info = SOUL_D->get_soul(which, "LIV STR", ({ this_object(), "______" }));
    outf("%s somebody ______ :\n     You'll see: %sTarget will see: %sOthers will see: %s\n",
      which, info[1][0], info[1][1], info[1][2]);
  }
  if ( data["LIV LIV"] )
  {
    info = SOUL_D->get_soul(which, "LIV LIV", ({ this_object(), load_object(CMD_DIR_PLAYER "/semote2") }));
    outf("%s somebody somebody :\n     You'll see: %sTarget will see: %sSecond will see: %sOthers will see: %s\n",
      which, info[1][0], info[1][1], info[1][2], info[1][3]);
  }
}

string query_pronoun(){  return "he";}

string query_reflexive(){ return "himself"; }

string query_subjective(){ return query_pronoun(); }

string query_objective(){ return "him"; }

string query_possessive(){ return "his"; }

string query_named_possessive(){  return "Somebody's"; }

string a_short(){ return "Somebody"; }

string short(){ return "Somebody"; }

string the_short(){ return "Somebody"; }

void player_menu_entry (string s)
{
  bare_init();
  main (s);
  done_outputing();
}
