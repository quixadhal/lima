/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

nomask private void main(string str) {
    if (!str) {
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
