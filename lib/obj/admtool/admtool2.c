/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**
** Provides various adminstration facilities for admins, domain lords, and
** wizards.
**
** 07-Jun-95.  Deathblade.  Created.
*/

#include <commands.h>
#include <security.h>
#include <daemons.h>

inherit __DIR__ "internal/base";

string module_name() {
    return "main";
}

class command_info array module_commands() {
    class command_info array ret = ({});
    string dir = base_name();
    string ourname;
    int idx = strsrch(dir, "/", -1);

    ourname = dir[idx+1..];
    dir = dir[0..idx];
    
    foreach (string file in get_dir(dir + "*.c") - ({ ourname + ".c" })) {
	string key, name, who;

	file = dir + file;

	key = file->module_key();
	name = file->module_name();
	who = file->module_user();
	
	if (key)
	    ret += ({ 
		new(class command_info, key : key, desc : name,
		    action : (: clone_object, file :),
		    who : who)
	    });
    }

    return ret;
}

protected void heading() {
    write("\n%^ADMTOOL_HEADING%^"
	  "--- Administration Tool ---\n"
	  "%^RESET%^\n");
}

class command_info array defaults() {
    return ({
	    new(class command_info), // blank line
	    new(class command_info,
		key : "q", 
		desc : "quit", 
		action : (: modal_pop(), destruct() :)),
	    new(class command_info,
		key : "?",
		desc : "help", 
		action : (: do_help :))
	});
}

void create()
{
    if ( clonep() )
    {
        if ( file_name(previous_object()) != CMD_OB_ADMTOOL )
	{
	    write("Illegal attempt to clone ADMTOOL\n");
            destruct(this_object());
            return;
        }

	::create();
    }
}

