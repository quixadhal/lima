/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

private void main(string arg)
{
    clone_object(HELPSYS)->begin_help(arg);
}

void player_menu_entry()
{
    main("");
}
