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
    string  output;

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
    output = "";
    matches = sort_array(matches,1);
    foreach(match in matches)
    {
	mudinfo = mudlist[match];
	output += implode(explode(match,"")," ") + "\n";
	output += "___________________________________\n";
	output += sprintf("Type: %s\n", mudinfo[8]);
	output += sprintf("Address: %s %d\n", mudinfo[1], mudinfo[2]);
	output += sprintf("Current Mudlib: %s\n", mudinfo[5]);
	output += sprintf("Base Mudlib: %s\n", mudinfo[6]);
	output += sprintf("Driver: %s\n", mudinfo[7]);
	output += "Status:  ";
	if(mudinfo[0] == -1)
	{
	    output += "Up\n";
	}
	else if(!mudinfo[0])
	{
	    output += "Down\n";
	}
	else
	{
	    output += sprintf("Down (Expected up in %d minutes)\n",
	      (mudinfo[0]+60)/60);
	}
	output += sprintf("Open Status: %s\n", mudinfo[9]);
	output += sprintf("Admin email: %s\n", mudinfo[10]);
	output += sprintf("TCP Imud port: %d\n", mudinfo[3]);
	output += sprintf("UDP Imud port: %d\n", mudinfo[4]);
	output += "Services available: ";
	if(!sizeof(mudinfo[11]))
	    output += "None given\n";
	else
	    output += implode(keys(mudinfo[11]), ", ") + "\n";
	if ( mudinfo[12] )
	    output += "Other data: " + implode(keys(mudinfo[12]), ", ") + "\n";
	output += "\n";
    }

    more(output);
}

