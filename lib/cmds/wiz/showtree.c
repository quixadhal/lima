/* Do not remove the headers from this file! see /USAGE for more info. */

// Quick hack by Beek

#include <mudlib.h>

inherit CMD;

varargs string print_tree(string file, string func, int indent) {
    object ob = find_object(file);
    string result;
    int found;
    
    if (file[0] != '/')
	file = "/" + file;
    
    if (!ob) {
	return sprintf("%*-' 'sCouldn't find object: " + file + "\n", indent * 2, "");
	return;
    }
    result = sprintf("%*-' 's" + file + ":\n", indent * 2, "");
    if (func && (function_exists(func, ob) + ".c") == file) {
	result += sprintf("%*-' 's  [defined]\n", indent * 2, "");
	found = 1;
    } else if (!func)
	found = 1;

    indent++;
    foreach (file in inherit_list(ob)) {
	string str = print_tree(file, func, indent);
	if (str != "") {
	    result += str;
	    found = 1;
	}
    }	
    return (found ? result : "");
}

// [str] cfile
private void main(mixed *arg) {
    string file, func;

    if (arg[1]) {
	file = arg[1];
	func = arg[0];
	write("Tracing definitions of '" + func + "' in " + file + "\n");
    } else {
	file = arg[0];
	func = 0;
    }

    write(print_tree(file, func));
}
