/* Do not remove the headers from this file! see /USAGE for more info. */

// Originally a 5 line hack by someone :)
// Modified by Beek when he successfully more'd /std/player (a directory)

#include <mudlib.h>

inherit CMD;



// specification is : more obj|file*
// we will recieve  : ({ array of files })

private void main(mixed *arg, mapping flags, string stdin)
{
    if(!arg[0])
      {
	if(!stdin)
	  {
	    out("Too few arguments.\n");
	    out("Usage: more file(s)\n");
	    return;
	  }
	else
	  {
	    more(stdin);
	    return;
	  }
      }
    foreach (mixed file in arg[0]) {
	if (objectp(file)) file = base_name(file) + ".c";
	if (!stringp(file)) continue;
	if (is_directory(file)) {
	    out(file + ": Is a directory.\n");
	    continue;
	}
	more_file(file);
    }
}

int help()
{
    write(wrap("Usage: more ( <filename>* | -c <chunksize> )\n"
	"The more command prints a file on the screen in managable chunks. "
	"Type '?' while within more for detailed information.\n"));
    return;
}
