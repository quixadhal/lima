/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

string *lines;

private void main(mixed * arg) {
    if ( is_file(arg[0]) )
    {
        lines = explode(read_file(arg[0]), "\n");
        write("Adding to: " + arg[0] + "\nuse '.' when done.\n-------------------------------------------------------\n");
        write(implode(lines[<10..<1], "\n") + "\n");
    }
    else
    {
        write("Creating: " + arg[0] + "\nuse '.' when done.\n-------------------------------------------------------\n");
    }

    clone_object(ADDTO_OB)->addto(arg[0]);
}

