/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(string str) {
    string rule;
    if ( !is_directory(wiz_dir(this_user())) )
    {
        write("Sorry, only full wizards can remove emotes.\n");
        return;
    }

    if (!str) {
	write("rmemote verb\n");
	return;
    }
    sscanf(str, "%s %s", str, rule);
    if (SOUL_D->remove_emote(str, rule))
	write("Removed.\n");
    else write("Failed.\n");
}

