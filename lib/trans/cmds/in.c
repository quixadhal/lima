/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(mixed *arg) {
    object old;
    string cmd;

    cmd = implode(arg[1], " ");;
    old = environment(this_body());

    this_body()->move(arg[0]);

    this_user()->force_me(cmd);

    if (old)
	this_body()->move(old);
    else
	outf("Could not move back to old room.\n");
    return;
}
