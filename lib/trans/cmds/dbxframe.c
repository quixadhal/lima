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

private mapping get_the_error(string str) {
    return unguarded(1, (: call_other, master(), "query_error", str :));
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
        out("dbxframe [<who>] <frame>\n");
	return;
    }
    if (!who) {
        frame = get_the_error(this_user()->query_userid());
    } else {
        frame = get_the_error(who);
	if (!frame && !find_body(who) && who != "last") {
	    out("No such player.\n");
	    return;
	}
    }
    if (!frame) {
	out("No error.\n");
	return;
    }
    if (num<0 || num>=sizeof(frame["trace"]))
	return out("No such frame.\n"), 0;
    frame = frame["trace"][num];

    outf("------%s:%i - %s(%s)\n", frame["program"], frame["line"],
      frame["function"],print_vars(frame["arguments"]));
    outf("locals: %s\n", print_vars(frame["locals"]));
    outf("----------------------------------------------------------------\n");

    before = (frame["line"] > 5 ? 5 : frame["line"] - 1);
    outf("%s=>%s%s",
      read_file("/"+frame["program"], frame["line"]-before, before),
      read_file("/"+frame["program"], frame["line"], 1),
      read_file("/"+frame["program"], frame["line"]+1, 5));

    return;
}
