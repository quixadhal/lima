/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** _idea.c
**
** Converted to new /std/reporter usage (Deathblade 4-Sep-94).
** Original by Rust (?)
*/

#include <mudlib.h>
inherit CMD;

private void main(string str)
{
    REPORTER_D->report_something("Idea", str);
}

void player_menu_entry()
{
    main("");
}
