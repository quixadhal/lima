/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: feelings, m_messages, addemote, showemote, stupidemote, targetemote
//USAGE:  rmemote <emote>
//        rmemote <emote>  <rule>
//
//This command allows you to remove an emote entirely, or in part
//by removing just a particular rule.
//
//rmemote kick
//
//   this will remove the entire emote kick and all its rules.
//
//rmemote kick LIV
//
//   this will remove only the rule and message for the LIV part of kick
//   It will leave all other rules as is.

#include <daemons.h>

inherit CMD;

private void main(string str) {
    string rule;
    if ( !is_directory(wiz_dir(this_user())) )
    {
        out("Sorry, only full wizards can remove emotes.\n");
        return;
    }

    if (!str) {
	out("rmemote verb\n");
	return;
    }
    sscanf(str, "%s %s", str, rule);
    if (SOUL_D->remove_emote(str, rule))
	out("Removed.\n");
    else out("Failed.\n");
}

