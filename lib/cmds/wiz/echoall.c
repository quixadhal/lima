/* Do not remove the headers from this file! see /USAGE for more info. */

// Looks like Rust.  Who couldn't write this in 2 seconds, tho?

#include <mudlib.h>

inherit CMD;


private void main(mixed * arg)
{
    string msg = implode(arg[0], " ");
    write("You echoall: " + msg + "\n");
    shout(msg + "\n");
}
