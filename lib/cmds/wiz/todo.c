/* Do not remove the headers from this file! see /USAGE for more info. */

/* todo.c -- log todos */

#include <mudlib.h>

inherit CMD;

#define TYPE "Todo"

private void main(string str)
{
    if ( !is_directory(wiz_dir(this_body())) )
    {
        write("Sorry, only full wizards are allowed to use this command.\n");
        return;
    }

#ifdef LOG_DONT_POST
  if(!str || str == "")
    REPORTER->begin_report(TYPE);
  else
    REPORTER->short_report(TYPE, str);
#else
  if (!str || str == "")
    REPORTER->begin_report(TYPE, TYPE+" report");
  else
    REPORTER->short_report(TYPE, TYPE+" report", str);
#endif
}
