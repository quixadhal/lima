/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** chanlist.c -- print available channels
**
** Print out the available channels in the system/intermud.  Note that
** the system allows any number of channels, so this will only print
** out some "suggested" channels.  This command is most effective for
** printing out a list of available Intermud channels.
**
** 950629, Deathblade: Created.
*/

#include <mudlib.h>

private nomask string fmt_imud_channel(string channel_name,
				       mixed * channel_data)
{
    string owner = channel_data[0];
    string type = ({ "unrestricted",
		     "restricted",
		     "filtered" })[channel_data[1]];

    if ( owner == "*" )
	owner = "no owner";

    return sprintf("%-19s Intermud: %s, %s\n",
		   channel_name, owner, type);
}

int main(string arg)
{
    string s;
    mapping chanlist = IMUD_D->query_chanlist();

    s = "Listing of available channels\n-----------------------------\n"
	"gossip              Local\n"
	"newbie              Local\n"
	"news                Local\n"
	;

    if ( wizardp(this_user()) )
	s += "wiz                 Wizard\n"
	     ;

    if ( GROUP_D->adminp(this_user()) )
	s += "admin               Admin\n"
	     ;

    s += implode(({ "" }) + keys(chanlist),
		 (: $1 + fmt_imud_channel($2, $(chanlist)[$2]) :));

/*
    s += implode(keys(chanlist),
		 (: $1 + sprintf("%-29s Intermud (owned by %s, %s)\n",
				 $2, $(chanlist)[$2][0],
				 $(({ "selectively banned",
					  "selective admission",
					  "filtered" }))[$(chanlist)[$2][1]]
		     ) :));
		     */

    new(MORE_OB)->more_string(s);
    
    return 1;
}
