/* Do not remove the headers from this file! see /USAGE for more info. */

// Originally a 5 line hack by someone :)
// Modified by Beek when he successfully more'd /std/player (a directory)

#include <mudlib.h>

inherit CMD;



// specification is : more obj|file*
// we will recieve  : ({ array of files })

private void main(mixed *arg, mapping flags, string stdin)
{
    array files = arg[0];
    if (!files) {
        if (!stdin) {
	    out("Too few arguments.\n");
	    out("Usage: more file(s)\n");
	} else
	    more(stdin, 0, 0, NO_ANSI);
	return;
    }
    
    files = filter(files, 
		   function(string file) {
	               if (!stringp(file)) return 0;
		       if (is_directory(file)) {
			   out(file + ": Is a directory.\n");
			   return 0;
		       }
		       return 1;
                   }
	);
    more_file(files, 0, 0, NO_ANSI);
}

void help()
{
    write("Usage: more ( <filename>* | -c <chunksize> )\n"
	"The more command prints a file on the screen in managable chunks. "
	"Type '?' while within more for detailed information.\n");
    return;
}
