/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

nomask private void main(string str) {
    if (!str) {
	write("stupidemote verb\n");
	return;
    }
    SOUL_D->add_emote(str, "", "$N $v"+str+".", 0);
    SOUL_D->add_emote(str, "STR", "$N $v"+str+" $o.", 0);
    out("Added.\n");
}
