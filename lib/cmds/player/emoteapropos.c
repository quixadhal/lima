/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE: emoteapropos string
//
//Finds all emotes/souls who's action contains the specified string.
//For example, "emoteapropos smile" would match every emote/soul that includes
//"smile" in the messages it sends.

#include <daemons.h>

inherit CMD;

nomask private void main(string str)
{
  if (!str)
  {
    write("emoteapropos string\n");
    return;
  }

  out(implode(({ "Souls that contain: " + str }) + SOUL_D->emote_apropos(str),"\n"));
}

void player_menu_entry(string str)
{
  bare_init();
  main(str);
  done_outputing();
}
