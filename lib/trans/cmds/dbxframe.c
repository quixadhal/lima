/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <security.h>

inherit CMD;

string print_vars(mixed *vars) {
    string *result = allocate(sizeof(vars));
    int i;

    for (i=0; i<sizeof(vars); i++) {
	if (mapp(vars[i]))
	    result[i] = "([ ... ])";
	else if (functionp(vars[i]))
	    result[i] = "(: ... :)";
	else if (intp(vars[i]))
	{
	    if (vars[i]) result[i]=vars[i]+"";
	    else if (nullp(vars[i])) result[i]="NULL";
	    else if (undefinedp(vars[i])) result[i]="UNDEFINED";
	    else result[i]="0";
	}
	else if (stringp(vars[i]))
	    result[i] = "\""+vars[i]+"\"";
	else if (pointerp(vars[i]))
	    result[i] = "({ ... })";
	else if (floatp(vars[i]))
	    result[i] = vars[i]+"";
	else if (bufferp(vars[i]))
	    result[i] = "<BUFFER>";
    }
    return implode(result, ", ");
}

private void
main(string str) {
    mapping frame;
    object ob;
    string who;
    int num;
    int i;
    int before;
    	
    if (!str || (sscanf(str, "%d", num)==0 && sscanf(str, "%s %d", who, num)==0)) {
        write("dbxframe [<who>] <frame>\n");
	return;
    }
    if (!who) {
	frame = master()->query_error(this_user()->query_real_name());
    } else {
	frame = master()->query_error(who);
	if (!frame && !find_body(who) && who != "last") {
	    write("No such player.\n");
	    return;
	}
    }
    if (!frame) {
	write("No error.\n");
	return;
    }
    if (num<0 || num>=sizeof(frame["trace"]))
	return write("No such frame.\n"), 0;
    frame = frame["trace"][num];

    printf("------%s:%i - %s(%s)\n", frame["program"], frame["line"],
      frame["function"],print_vars(frame["arguments"]));
    printf("locals: %s\n", print_vars(frame["locals"]));
    printf("----------------------------------------------------------------\n");

    before = (frame["line"] > 5 ? 5 : frame["line"] - 1);
    printf("%s=>%s%s",
      read_file("/"+frame["program"], frame["line"]-before, before),
      read_file("/"+frame["program"], frame["line"], 1),
      read_file("/"+frame["program"], frame["line"]+1, 5));

    return;
}
