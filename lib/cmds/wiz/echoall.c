/* Do not remove the headers from this file! see /USAGE for more info. */

// Looks like Rust.  Who couldn't write this in 2 seconds, tho?

#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string orig_input, mixed * arg)
{
    string msg = implode(arg[0], " ");
    out("You echoall: " + msg + "\n");
    tell(users() - ({ this_user() }), msg + "\n");
}
