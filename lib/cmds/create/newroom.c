/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

private void main(array args) {
    string fname = args[0];
    
    if (fname[4..] != ".scr")
	fname += ".scr";
    if (file_size(fname) != -1) {
	write("File exists!\n");
	return;
    }
    write_file(fname, @END
is=room
brief=A new room (change with 'setbrief')
long=There is nothing here.  Describe this room using 'describeroom'.  Add objects with 'addobject'.  Add exits with 'addexit'.
END);
    write("Done (moving you there).\n");
    this_body()->move(fname);
}
