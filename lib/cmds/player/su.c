/* Do not remove the headers from this file! see /USAGE for more info. */

// _su.c   (switch user cmd)
// Rust@ZorkMUD
// Megaboz@ZorkMUD added help 5-5-94

#include <daemons.h>
#include <mudlib.h>

inherit CMD;

private void main(string arg)
{
    string name, race;
    if (!arg) {
	name = this_user()->query_userid();
	race = 0;
    } else
    if (sscanf(arg, "(%s)%s", race, name) == 2) {
	if (name == "") name = this_user()->query_userid();
    } else {
	name = arg;
	race = 0;
    }

    if (race) {
	string array dirs = ({ DIR_RACES });
	while (sizeof(dirs)) {
	    foreach (string file in get_dir(dirs[0])) {
		if (race + ".c" == file) race = dirs[0] + file;
		if (file_size(dirs[0] + file) == -2)
		    dirs += ({ dirs[0] + file + "/" });
	    }
	    dirs = dirs[1..];
	}
	if (race[<2..] != ".c") {
	    out("No such race.\n");
	    return;
	}
    }

    if ( this_body() )
    {
	this_body()->save_me();  
	this_body()->save_autoload();  
    }

    this_user()->switch_user(name, race);
}
