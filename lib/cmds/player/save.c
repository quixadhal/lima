/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** save.c -- simple player save cmd
**
** 950711, Deathblade: Created.
*/

#include <mudlib.h>
inherit CMD;

private void main(string arg)
{
    this_body()->save_me();
    out("Saved.\n");

    return;
}
