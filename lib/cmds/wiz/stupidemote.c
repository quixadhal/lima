/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

nomask int main(string str) {
    if (!str) {
	write("stupidemote verb\n");
	return 0;
    }
    SOUL_D->add_emote(str, "", "$N $v"+str+".", 0);
    SOUL_D->add_emote(str, "STR", "$N $v"+str+" $o.", 0);
    write("Added.\n");
    return 1;
}
