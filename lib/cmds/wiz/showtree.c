/* Do not remove the headers from this file! see /USAGE for more info. */

// Quick hack by Beek


inherit CMD;

varargs string print_tree(string file, string func, int indent) {
    object ob;
    string result;
    int found;
    
    if( !file )
    {
        return "You need to specify an object.\n";
    }
    
ob = find_object( absolute_path(file, get_user_variable("pwd")));
    
    if (!ob) {
    return repeat_string("  ", indent) + "Couldn't find object: " + file + "\n";
    }
    result = repeat_string("  ", indent) + file + ":\n";
    if (func && (function_exists(func, ob) + ".c") == file) {
    result += repeat_string("  ", indent) + "  [defined]\n";
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
	out("Tracing definitions of '" + func + "' in " + file + "\n");
    } else {
	file = arg[0];
	func = 0;
    }

    out(print_tree(file, func));
}
