/* Do not remove the headers from this file! see /USAGE for more info. */

// Originally a 5 line hack by someone :)
// Modified by Beek when he successfully more'd /std/player (a directory)

#include <mudlib.h>

inherit DAEMON;
inherit M_GLOB;



// specification is : more file*
// we will recieve  : ({ array of files })

int main(mixed *arg)
{
    clone_object(MORE_OB)->more_files(filter(arg[0],(:is_file:)));
     return 1;
}

int help()
{
    write(wrap("Usage: more ( <filename>* | -c <chunksize> )\n"
	"The more command prints a file on the screen in managable chunks. "
	"Type '?' while within more for detailed information.\n"));
    return 1;
}
