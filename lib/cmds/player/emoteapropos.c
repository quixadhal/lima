/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

nomask private void main(string str) {
    if (!str) {
        write("emoteapropos string\n");
        return;
    }

    more(({ "Souls that contain: " + str }) + SOUL_D->emote_apropos(str));
}

void player_menu_entry(string str)
{
    main(str);
}
