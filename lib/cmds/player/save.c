/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** save.c -- simple player save cmd
**
** 950711, Deathblade: Created.
*/

int main(string arg)
{
    this_body()->save_me();
    write("Saved.\n");

    return 1;
}
