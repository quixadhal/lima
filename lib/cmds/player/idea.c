/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** _idea.c
**
** Converted to new /std/reporter usage (Deathblade 4-Sep-94).
** Original by Rust (?)
*/

#include <mudlib.h>

#define LOG_FILE  "IDEAS"
#define TYPE "Idea"

int main(string str)
{
#ifdef LOG_DONT_POST
  if(!str || str == "")
    REPORTER->begin_report(LOGFILE, TYPE);
  else
    REPORTER->short_report(LOGFILE, TYPE, str);
#else
    if (!str || str == "")
        REPORTER->begin_report(IDEA_NEWSGROUP, TYPE, TYPE+" report");
    else
         REPORTER->short_report(IDEA_NEWSGROUP, TYPE, str, TYPE+" report");
#endif
    return 1;
}
