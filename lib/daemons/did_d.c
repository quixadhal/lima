/* Do not remove the headers from this file! see /USAGE for more info. */

/* Daemon to keep track of what people have been doing.
 * -Beek
 */
#include <mudlib.h>
#include <security.h>

inherit M_ACCESS;


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

void
dump_did_info(int time) {
    int index;
    string array output = ({"Changes since you last logged in:",
                            "*************************************\n"});

    index = sizeof(did) - 1;
    while (index > 0 && time < did[index][0]) index--;
    index++;

    if(index >= sizeof(did))
      return;

    for ( ; index < sizeof(did); index++) {
        output += 
	  ({iwrap(sprintf("%s: %s\n", ctime(did[index][0]), did[index][1]))});
    }
    new(MORE_OB)->more_lines(output, 3);
}

varargs void
print_changelog_to_file(string file, int time, int show_date)
{
    int index;
    string array output = ({});

    index = sizeof(did) - 1;
    while (index > 0 && time < did[index][0]) index--;
    index++;

    if(show_date) {
      for ( ; index < sizeof(did); index++) {
        output += 
	  ({iwrap(sprintf("%s: %s\n", ctime(did[index][0]), did[index][1]))});
      }
    }
    else {
      for ( ; index < sizeof(did); index++) {
        output += 
	  ({iwrap(sprintf("%s\n", did[index][1]))});
      }
    }
    write_file(file, implode(output,"\n"));
}
  
