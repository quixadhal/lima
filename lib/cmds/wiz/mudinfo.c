/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$see: mudlist
//USAGE:  mudinfo <mudname>
//
//This command will give you specific information about a mud.
//
//mudinfo ideaexchange
//
//I d e a E x c h a n g e
//___________________________________
//Type: LPMud
//Address: 199.199.122.10 7890
//Current Mudlib: Foundation IIr1
//Base Mudlib: Foundation IIr1
//Status:  Up
//Open Status: open for public
//TCP Imud port: 7895
//UDP Imud port: 7898
//Services available: tell, who, rcp, http, locate, ftp, channel, finger
//
//Driver: MudOS v21.6a10
//
//This command can also be accessed through the menu.
//
//In order for this to be available, both this mud and the target mud must
//have been connected to the intermud 3 server at the same time.
//The information is saved, so it is not necessary for either mud to be
//currently connected, but the information could be out-of-date in such
//circumstances.

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

