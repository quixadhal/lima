/* Do not remove the headers from this file! see /USAGE for more info. */

// Utility functions for operating on .scr files.

inherit CMD;

/* So we don't get interpreted as a command.  If we were to return 1, then
 * our children would not be commands either.  Instead, we return our filename
 * which won't match for children.
 */
mixed not_a_cmd() {
    return __FILE__[0..<3]; // no .c
}

string get_file_name() {
    object env = environment(this_body());
    mixed fn;
    
    if (!env) {
	write("You're nowhere!\n");
	return 0;
    }
    fn = file_name(env);
    if (fn[<4..] != ".scr") {
	write("You are not standing in an LPscript room.\n");
	return 0;
    }
    return fn;
}

int add_exit(string fname, string dir, string value) {
    string text = read_file(fname);
    string before, values, after;
    array parts;
    int i, found;
    
    if (!text) {
	write("Failed (could not read file).\n");
	return 0;
    }
    parts = explode(text, "\nexits=");
    switch (sizeof(parts)) {
    case 1:
	before = parts[0];
	values = "";
	after = "";
	break;
    case 2:
	before = parts[0];
	after = parts[1];
	i = 0;
	while (after[i] == ' ') i++;
	after = after[i..];
	if (after[0] == '\n') {
	    i = strsrch(after, "\nend\n");
	    if (i == -1) {
		write("Failed (could not find end of 'exits').\n");
		return 0;
	    }
	    values = after[0..i-1];
	    after = after[i+5..];
	} else {
	    i = member_array('\n', after);
	    if (i == -1) {
		write("Failed (could not find end of 'exits').\n");
		return 0;
	    }
	    values = after[0..i-1];
	    after = after[i+1..];
	}
	break;
    default:
        write("Failed (found >1 instance of 'exits').\n");
        return 0;
    }
    parts = explode(values, "\n");
    for (i = 0; i < sizeof(parts); i++) {
	string item = parts[i];
	int ind = 0, p;

	while (item[ind] == ' ')
	    ind++;
	p = member_array(':', item);
	if (item[ind..p-1] == dir) {
	    parts[i] = item[0..p+1] + value;
	    found = 1;
	}
    }
    if (!found) {
	int ind = 0;
	if (sizeof(parts)) {
	    while (parts[0][ind] == ' ')
		ind++;
	} else
	    ind = 2;
	parts += ({ repeat_string(" ", ind) + dir + ": " + value });
    }
    write_file(fname, before + "\nexits=\n" + implode(parts, "\n") + "\nend\n" + after, 1);
    return 1;
}

int change_attribute(string fname, string attr, string what) {
    string text = read_file(fname);
    string before, after;
    array parts;
    int i;
    
    if (!text) {
	write("Failed (could not read file).\n");
	return 0;
    }
    parts = explode(text, "\n"+attr+"=");
    switch (sizeof(parts)) {
    case 1:
	before = "";
	after = parts[0];
	break;
    case 2:
	before = parts[0];
	after = parts[1];
	i = 0;
	while (after[i] == ' ') i++;
	after = after[i..];
	if (after[0] == '\n') {
	    i = strsrch(after, "\nend\n");
	    if (i == -1) {
		write("Failed (could not find end of '"+attr+"').\n");
		return 0;
	    }
	    after = after[i+5..];
	} else {
	    i = member_array('\n', after);
	    if (i == -1) {
		write("Failed (could not find end of '"+attr+"').\n");
		return 0;
	    }
	    after = after[i+1..];
	}
	break;
    default:
        write("Failed (found >1 instance of '"+attr+"').\n");
        return 0;
    }
    if (member_array('\n', what) == -1)
	write_file(fname, before + "\n" + attr + "=" + what + "\n" + after, 1);
    else
	write_file(fname, before + "\n" + attr + "=\n  " + replace_string(what, "\n", "\n  ") + "\nend\n" + after, 1);
    return 1;
}

void update(string fname) {
    object ob;
    array tosave;
    
    if (ob = find_object(fname)) {
	tosave = filter(all_inventory(ob), function(object ob) {
	    if (ob->query_link() && interactive(ob->query_link())) {
		ob->move(VOID_ROOM);
		return 1;
	    }
	});
	destruct(ob);
	tosave->move(load_object(fname));
    }
}
