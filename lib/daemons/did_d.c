/* Do not remove the headers from this file! see /USAGE for more info. */

/* Daemon to keep track of what people have been doing.
 * -Beek
 */
#include <mudlib.h>
#include <security.h>

inherit M_ACCESS;
inherit M_GLOB;



#define SAVE_FILE "/data/daemons/did_d"

mixed *did = ({ });

void
create() {
    restore_object(SAVE_FILE, 1);
}

void
someone_did(string str) {
    object *who;
    object ob;
    int i;

    str = this_body()->query_name() + " " + str;
    did += ({ ({ time(), str }) });
    unguarded(1, (: save_object, SAVE_FILE :));

    who = users() - ({ this_user() });
    who = who->query_body() - ({ 0 });
    who->ilog_hook(str);
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
	    output += explode(iwrap(sprintf("%s: %s",
					    ctime(did[index][0]),
					    did[index][1])), "\n") + ({ "" });
    }

    more(output, 0, continuation);
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
        output += iwrap(sprintf("%s: %s\n",
				ctime(did[index][0]),
				did[index][1])) + "\n";
      }
    }
    else {
      for ( ; index < sizeof(did); index++) {
        output += iwrap(sprintf("%s\n", did[index][1])) + "\n";
      }
    }
    write_file(file, output);
}
