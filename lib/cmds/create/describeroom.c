/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "scr_command";
inherit M_INPUT;

string sofar;
string fname;

private void continue_describe(string this) {
    if (this == ".") {
	modal_pop();
	if (sofar == "") {
	    write("Description unchanged.\n");
	} else {
	    if (change_attribute(fname, "long", sofar))
		write("Done.\n");
	    update(fname);
	}
	destruct();
    } else {
	if (this == "")
	    sofar += "\n";
	else
	    sofar += this + " ";
    }
}

void begin_description(string fn) {
    sofar = "";
    fname = fn;
    modal_push((: continue_describe :), "]");
}
    
private void main() {
    string fname;

    if (!(fname = get_file_name()))
	return;

    write("Begin typing description.  Use '.' alone on a line when done.\n");
    new(file_name())->begin_description(fname);
}
