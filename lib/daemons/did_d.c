/* Do not remove the headers from this file! see /USAGE for more info. */

/* Daemon to keep track of what people have been doing.
 * -Beek
 */
#include <mudlib.h>
#include <security.h>
#include <net/http_d.h>

inherit M_ACCESS;
inherit M_GLOB;



#define SAVE_FILE "/data/daemons/did_d"

mixed *did = ({ });

void
create() {
    restore_object(SAVE_FILE, 1);
}

int someone_did(string str) {
    object *who;
    object ob;
    int i;

    if( !is_directory( wiz_dir( this_body())))
    {
	write( "Sorry, but only full wizards may use the didlog.\n");
        return 0;
    }
    str = capitalize( this_body()->query_userid()) + " " + str;
    did += ({ ({ time(), str }) });
    unguarded(1, (: save_object, SAVE_FILE :));

    who = filter_array( users(), (: wizardp( $1 ):)) - ({ this_user() });
    who = who->query_body() - ({ 0 });
    who->ilog_hook(str);
    return 1;
}

varargs void dump_did_info(int time, string * header, string pattern,
  function continuation)
{
    int index;
    string * output = header;
    mixed *matches;

    if ( !header )
	output = ({ "Change Log",
	  "**********",
	  "" });

    index = sizeof(did) - 1;
    while (index > 0 && time < did[index][0])
	index--;

    /* we went too far back in time. move up one entry. */
    if ( index )
	index++;

    if ( index >= sizeof(did) )
    {
	if ( continuation )
	    evaluate(continuation);
	return;
    }

    for ( ; index < sizeof(did); index++)
    {
	if ( !pattern || regexp(did[index][1], translate(pattern,1)) )
	    output += explode(sprintf("%s: %s",
		ctime(did[index][0]),
		did[index][1]), "\n") + ({ "" });
    }

    more(output, 0, continuation, NO_ANSI);
}

varargs string get_did_info(int time, string * header, string pattern,
  function continuation)
{
    int index;
    string * output = header;
    mixed *matches;

    if ( !header )
	output = ({ "Change Log",
	  "**********",
	  "" });

    index = sizeof(did) - 1;
    while (index > 0 && time < did[index][0])
	index--;

    /* we went too far back in time. move up one entry. */
    if ( index )
	index++;

    if ( index >= sizeof(did) )
    {
	if ( continuation )
	    evaluate(continuation);
	return implode(output, "\n");
    }

    for ( ; index < sizeof(did); index++)
    {
	if ( !pattern || regexp(did[index][1], translate(pattern,1)) )
	    output += explode(sprintf("%s: %s",
		ctime(did[index][0]),
		did[index][1]), "\n") + ({ "" });
    }

    return implode(output,"\n");
}

varargs void
print_changelog_to_file(string file, int time, int show_date)
{
    int index;
    string output = "";

    index = sizeof(did) - 1;
    while (index > 0 && time < did[index][0]) index--;
    index++;

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

private string who_link (string name)
{
    return sprintf ("<a href=http://%s:%d/cgi/who.c?=%s>%s</a>",
      __HOST__, HTTP_PORT, lower_case (name),
      name);
}

string get_changelog_for_web(int dont_link_names)
{
    int 		index = 0;
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
	  ctime (did[index][0]), 
	  dont_link_names ? name : who_link(name), 
	  entry[space..]);
    }
    return output + "</DL>\n";
}

void print_weblog_to_file(string fname)
{
    write_file(fname, get_changelog_for_web(1));
}
