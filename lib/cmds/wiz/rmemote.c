/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

int main(string str) {
    string rule;
    if (!str) {
	write("rmemote verb\n");
	return 0;
    }
    sscanf(str, "%s %s", str, rule);
    if (SOUL_D->remove_emote(str, rule))
	write("Removed.\n");
    else write("Failed.\n");
    return 1;
}

