/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

nomask int main(string str) {
    if (!str) {
        write("emoteapropos string\n");
        return 0;
    }
    write("Souls that contain: " + str + "\n");
    write(implode(SOUL_D->emote_apropos(str), "\n")+"\n");
    return 1;
}
