/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

nomask private void main(string str) {
    if (!str) {
        write("emoteapropos string\n");
        return;
    }
    write("Souls that contain: " + str + "\n");
    write(implode(SOUL_D->emote_apropos(str), "\n")+"\n");
    return;
}
