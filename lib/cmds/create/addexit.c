/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "scr_command";    

private string reverse(string dir) {
    switch (dir) {
    case "north" : return "south";
    case "northeast" : return "southwest";
    case "east" : return "west";
    case "southeast" : return "northwest";
    case "south" : return "north";
    case "southwest" : return "northeast";
    case "west" : return "east";
    case "northwest" : return "southeast";
    case "up" : return "down";
    case "down" : return "up";
    }
}

private void main(array args) {
    string dir = args[0];
    string where = args[1];
    string fname;
    
    if (!(fname = get_file_name()))
	return;
    if (file_size(where + ".c") >= 0)
	where += ".c";
    if (file_size(where + ".scr") >= 0)
	where += ".scr";
    if (file_size(where) < 0) {
	write("Could not find file '" + where + "'.\n");
	return;
    }
    if (add_exit(fname, dir, where))
	write("Done.\n");
    update(fname);
    if (where[<4..] == ".scr") {
	string dir2 = reverse(dir);
	if (!dir2) {
	    write("No return path added; inverse of '" + dir + "' unknown.\n");
	    return;
	}
	write("Adding return path ...\n");
	if (add_exit(where, dir2, fname))
	    write("Done.\n");
	update(where);
    } else {
	write("No return path added; destination not written in LPscript.\n");
    }
}
