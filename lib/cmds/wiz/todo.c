/* Do not remove the headers from this file! see /USAGE for more info. */

/* todo.c -- log todos */

#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string str)
{
    if ( !is_directory(wiz_dir(this_body())) )
    {
        out("Sorry, only full wizards are allowed to use this command.\n");
        return;
    }

    REPORTER_D->report_something("Todo", str);
}

void player_menu_entry()
{
    main("");
}
