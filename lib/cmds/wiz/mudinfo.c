/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_REGEX;
inherit M_GLOB;


private void main(string arg)
{
    mapping mudlist = IMUD_D->query_mudlist();
    string* muds = keys(mudlist);
    string* matches;
    string  match;
    mixed*  mudinfo;

    if(!arg || arg == "")
    {
	arg = mud_name();
    }
    matches = insensitive_regexp(muds, "^"+translate(arg));
    if(!sizeof(matches))
    {
	printf("No muds out of %d match that pattern.\n", sizeof(mudlist));
	return;
    }
    matches = sort_array(matches,1);
    foreach(match in matches)
    {
	mudinfo = mudlist[match];
	out(implode(explode(match,"")," ") + "\n");
	out("___________________________________\n");
	out(sprintf("Type: %s\n", mudinfo[8]));
	out(sprintf("Address: %s %d\n", mudinfo[1], mudinfo[2]));
	out(sprintf("Current Mudlib: %s\n", mudinfo[5]));
	out(sprintf("Base Mudlib: %s\n", mudinfo[6]));
	out(sprintf("Driver: %s\n", mudinfo[7]));
	out("Status:  ");
	if(mudinfo[0] == -1)
	{
	    out("Up\n");
	}
	else if(!mudinfo[0])
	{
	    out("Down\n");
	}
	else
	{
	    out(sprintf("Down (Expected up in %d minutes)\n",
	      (mudinfo[0]+60)/60));
	}
	out(sprintf("Open Status: %s\n", mudinfo[9]));
	out(sprintf("Admin email: %s\n", mudinfo[10]));
	out(sprintf("TCP Imud port: %d\n", mudinfo[3]));
	out(sprintf("UDP Imud port: %d\n", mudinfo[4]));
	out("Services available: ");
	if(!sizeof(mudinfo[11]))
	    out("None given\n");
	else
	    out(implode(sort_array(keys(mudinfo[11]) - ({ 0 }), 1), ", ") + "\n");
	if ( mudinfo[12] )
	    out("Other data: " + implode(keys(mudinfo[12]), ", ") + "\n");
	out("\n");
    }
}

