/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(){

    object room = environment(this_body());
    string* directions;
    string base;

    directions = room->query_exit_directions(1);
    if(!sizeof(directions)) {
	out("There are no exits to this room.\n");
	return;
    }

    out("Current exits:\n");

    foreach (string dir in directions) {
	mixed dest;
	object o;
	string Short;
	
	dest = room->query_exit_value(dir);
	if (dest) {
	    if (dest[0] == '#') {
		outf("%s:  %s", dir, dest);
		continue;
	    } else
	    if (o = load_object(dest))
		Short = o->short() || "";
	    else
		Short = "Under construction";
	} else
	    Short = "None";
	outf("%s:  %s (%s)\n", dir, Short, dest);
    }
}

int help(){
    write("Usage:\n"
      "The exits command shows what it says.\n");
    return 1;
}
