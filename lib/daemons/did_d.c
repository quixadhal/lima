/* Do not remove the headers from this file! see /USAGE for more info. */

/* Daemon to keep track of what people have been doing.
 * -Beek
 */

#include <security.h>
#include <ports.h>

inherit M_DAEMON_DATA;
inherit M_GLOB;

mixed *did = ({ });

int someone_did(string str) {
    object array who;

    if( !is_directory( wiz_dir( this_user())))
    {
	write( "Sorry, but only full wizards may use the didlog.\n");
        return 0;
    }
    str = capitalize( this_user()->query_userid()) + " " + str;
    did += ({ ({ time(), str }) });
    save_me();

    who = filter_array( users(), (: wizardp( $1 ):)) - ({ this_user() });
    who -= ({ 0 });
    who->deliver_didlog_message(str);

    return 1;
}

void someone_didnt()
{
    if( !check_privilege("Mudlib:daemons")) error( "Only Admins may remove didlogs.\n");
    if( sizeof(did)) did = did[0..<2];
    save_me();
}


private nomask int start_index(int after)
{
    int index = sizeof(did);

    while ( index > 0 && did[index-1][0] > after )
	index--;

    return index;
}



private nomask string * get_entries(int after, string * header, string pattern)
{
    int index = start_index(after);
    string * output = header;

    if ( index >= sizeof(did) )
	return 0;

    if ( !header )
	output = ({ "Change Log",
		    "**********",
		    "" });

    if ( pattern )
	pattern = translate(pattern, 1);

    for ( ; index < sizeof(did); index++ )
    {
	if ( !pattern || regexp(did[index][1], pattern) )
	    output += explode(sprintf("%s: %s",
				      ctime(did[index][0]),
				      did[index][1]),
			      "\n") + ({ "" });
    }

    return output;
}

string array dump_entries()
{
  return did;
}

varargs void dump_did_info(int after, string * header, string pattern,
			   function continuation)
{
    string * output = get_entries(after, header, pattern);

    if ( !output )
    {
	if ( continuation )
	    evaluate(continuation);
    }
    else
    {
	more(output, 0, continuation, NO_ANSI);
    }
}

varargs string get_did_info(int after, string * header, string pattern,
			    function continuation)
{
    string * output = get_entries(after, header, pattern);

    if ( !output )
    {
	if ( continuation )
	    evaluate(continuation);
	return "No recent changes.\n";
    }

    return implode(output, "\n");
}

varargs void print_changelog_to_file(string file, int after, int show_date)
{
    int index = start_index(after);
    string output = "";

    if(show_date) {
	for ( ; index < sizeof(did); index++) {
	    output += sprintf("%s: %s\n",
			      ctime(did[index][0]),
			      did[index][1]) + "\n";
	}
    }
    else {
	for ( ; index < sizeof(did); index++) {
	    output += sprintf("%s\n", did[index][1]) + "\n";
	}
    }
    write_file(file, output);
}

private string who_link(string name)
{
    return sprintf("<a href=http://%s:%d/cgi/who.c?=%s>%s</a>",
		   __HOST__, PORT_HTTP, lower_case(name), name);
}

string get_changelog_for_web(int dont_link_names)
{
    int		index = 0;
    string	output = "<DL>\n";
    int		space;
    string	entry;
    string	name;

    for ( ; index < sizeof (did); index++)
    {
	entry = did[index][1];
	space = strsrch (entry, " ");
	name = entry[0..space-1];
	output += sprintf ("<DT><em>%s</em>: <strong>%s</strong>\n<DD>%s<p>\n",
			   ctime(did[index][0]), 
			   dont_link_names ? name : who_link(name), 
			   entry[space..]);
    }
    return output + "</DL>\n";
}

void print_weblog_to_file(string fname)
{
    write_file(fname, get_changelog_for_web(1));
}

