/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

nomask int main(string str) {
    if (!str) {
	write("targetemote verb\n");
	return 0;
    }
    SOUL_D->add_emote(str, 0, "$N $v"+str+".", 0);
    SOUL_D->add_emote(str, 1, "$N $v"+str+" $o.", 0);
    SOUL_D->add_emote(str, "OBJ", "$N $v"+str+" at $o.", 0);
    SOUL_D->add_emote(str, "LIV", "$N $v"+str+" at $t.", 0);
    write("Added.\n");
    return 1;
}
