/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_GLOB;
inherit M_REGEX;

/*
** Pairs of info for each column.  ( header-index, field-width )
** The header indices can be seen from the headers[] array below.
**
** NOTE: at the moment, the first two elements must specify the "Up"
**       state and the mud name
*/
#define INFO_DRIVERTYPE ({ \
    ({ 0, 2 }), ({ 11, 18 }), \
    ({8,8}),({ 7, 20 }), ({ 5, 18 })})

#define INFO_MUDTYPE	({ \
    ({ 0, 2 }), ({ 11, 18 }), \
    ({8,8}),({ 1, 15 }), ({ 2, 5 }), ({ 5, 18 })})

#define INFO_DEFAULT	({ \
    ({ 0, 2 }), ({ 11, 15 }), \
    ({1, 15}), ({2,5}), ({5, 12}), ({ 9, 17 }) })


static private string * headers = ({
    "Up",                                 // 0
    "Address",                            // 1
    "",		/* port */                // 2
    "",		/* TCP OOB port */        // 3
    "",		/* UDP OOB port */        // 4
    "Lib",                                // 5
    "Base Lib",                           // 6
    "Driver",                             // 7
    "Type",                               // 8
    "Open Status",                        // 9
    "Admin Email",                        // 10

    "Mud",	/* special... mudname. not part of the mud info */
});


private void main(mixed *arg, mapping flags)
{
    mapping	mudlist = IMUD_D->query_mudlist();
    string*	muds = keys(mudlist);
    string*	matches;
    int		matched;
    int		upcount;
    string	match;
    mixed*	mudinfo;
    string	output;
    mixed *	info;
    string	format;
    string	search_str;

    if ( !arg || stringp(arg) )
    {
        arg = ({ 0 });
	flags = ([ ]);
    }

    if ( !arg[0] )
	matches = muds;
    else
    {
	matches = insensitive_regexp(muds, "^" + translate(arg[0]));
	if ( !sizeof(matches) )
	{
	    outf("No muds out of %d match that pattern.\n", sizeof(mudlist));
	    return;
	}
    }

    if ( flags["t"] )
	info = INFO_MUDTYPE;
    else if ( flags["d"] )
	info = INFO_DRIVERTYPE;
    else
	info = INFO_DEFAULT;

    search_str = flags["s"];
    if ( search_str )
	search_str = lower_case(search_str);

    format = implode(info, (: $1 + sprintf("%%-%ds  ", $2[1]) :), "");
    format[<1] = '\n';

    matched = sizeof( matches );

    if ( flags["a"] )
    {
	upcount = implode(matches, (: $1 + ($(mudlist)[$2][0] == -1) :), 0);
    }
    else
    {
	matches = filter_array(matches, (: $(mudlist)[$1][0] == -1 :));
    upcount = sizeof( matches );
    }

    output = "";
    if ( wizardp(this_user()) )
	output += "Type mudinfo <mudname> for more info on a mud.\n";
    output += sprintf(format + repeat_string("-", 76) + "\n",
		      map_array(info, (: $(headers)[$1[0]] :))...);

    foreach ( match in sort_array(matches, 1) )
    {
        string	line;

	mudinfo = mudlist[match];
	line = sprintf(format,
		       mudinfo[0] == -1 ? "U" : "D",
		       match[0..info[1][1]-1],
		       map_array(info[2..],
				 (: ($(mudinfo)[$1[0]]+"")[0..$1[1]-1] :)
			   )...);
	if ( search_str )
	{
	    if ( regexp(lower_case(line), search_str) )
	    {
		output += line;
	    }
	    else
	    {
		--matched;
		if ( mudinfo[0] == -1 )
		    --upcount;
	    }
	}
	else
	{
	    output += line;
	}

    }

    output = sprintf("%d matches out of %d muds. %d %s UP.\n",
		     matched, sizeof(mudlist), upcount, (upcount == 1) ? "is" : "are") + output;

    out(output);
}

void player_menu_entry()
{
  bare_init();
  main(0, 0);
  done_outputing();
}
