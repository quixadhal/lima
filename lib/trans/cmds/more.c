/* Do not remove the headers from this file! see /USAGE for more info. */

// Originally a 5 line hack by someone :)
// Modified by Beek when he successfully more'd /std/player (a directory)

#include <mudlib.h>

inherit CMD;



// specification is : more file*
// we will recieve  : ({ array of files })

private void main(mixed *arg)
{
    more_file(filter(arg[0], (:is_file:)));
}

int help()
{
    write(wrap("Usage: more ( <filename>* | -c <chunksize> )\n"
	"The more command prints a file on the screen in managable chunks. "
	"Type '?' while within more for detailed information.\n"));
    return;
}
