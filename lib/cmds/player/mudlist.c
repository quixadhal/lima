/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_GLOB;
inherit M_REGEX;

#define INFO_DEFAULT	({ \
    ({ 0, 2 }), ({ 10, 16 }), \
    ({ 1, 15 }), ({ 2, 5 }), ({ 5, 15 }), ({ 9, 19 }) })

#define INFO_MUDTYPE	({ \
    ({ 0, 2 }), ({ 10, 20 }), \
    ({ 1, 15 }), ({ 2, 5 }), ({ 8, 8 }), ({ 9, 19 }) })


static private string * headers = ({
    "Up",
    "Address",
    "",		/* port */
    "",		/* TCP OOB port */
    "",		/* UDP OOB port */
    "Lib",
    "Base Lib",
    "Driver",
    "Type",
    "Open Status",

    "Mud",	/* special... not part of the mud info */
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

    if ( !arg[0] )
	matches = muds;
    else
    {
	matches = insensitive_regexp(muds, "^" + translate(arg[0]));
	if ( !sizeof(matches) )
	{
	    printf("No muds out of %d match that pattern.\n", sizeof(mudlist));
	    return;
	}
    }

    if ( flags["t"] )
	info = INFO_MUDTYPE;
    else
	info = INFO_DEFAULT;

    format = implode(info, (: $1 + sprintf("%%-%ds ", $2[1]) :), "");
    format[<1] = '\n';

    matched = sizeof(matches);

    if ( flags["a"] )
    {
	upcount = implode(matches, (: $1 + ($(mudlist)[$2][0] == -1) :), 0);
    }
    else
    {
	matches = filter_array(matches, (: $(mudlist)[$1][0] == -1 :));
	upcount = sizeof(matches);
    }

    output = sprintf("%d matches out of %d muds. %d are UP.\n",
		     matched, sizeof(mudlist), upcount);
    if ( wizardp(this_user()) )
	output += "Type mudinfo <mudname> for more info on a mud.\n";
    output += sprintf(format + "%76'-'s\n",
		      map_array(info, (: $(headers)[$1[0]] :))...,
		      "");

    foreach ( match in sort_array(matches, 1) )
    {
	mudinfo = mudlist[match];
	output += sprintf(format,
			  mudinfo[0] == -1 ? "U" : "D",
			  match[0..info[1][1]-1],
			  map_array(info[2..],
				    (: ($(mudinfo)[$1[0]]+"")[0..$1[1]-1] :)
					)...);
    }
  
    clone_object(MORE_OB)->more_string(output);
}
