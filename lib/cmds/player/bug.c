/* Do not remove the headers from this file! see /USAGE for more info. */

// _bug.c
// Rust wrote it
// Megaboz@ZorkMUD attached header 5-5-94
// Converted to use new /std/reporter superclass (Deathblade 4-Sep-94)
// Beek made one line reports work right 11/14/94
// Rust made them post news

#include <mudlib.h>

#define TYPE "Bug"
#define LOGFILE "BUGS"
int main(string str)
{
#ifdef LOG_DONT_POST
  if(!str || str == "")
    REPORTER->begin_report(LOGFILE, TYPE);
  else
    REPORTER->short_report(LOGFILE, TYPE, str);
#else
    if (!str || str == "")
        REPORTER->begin_report(BUG_NEWSGROUP, TYPE, TYPE+" report");
    else
         REPORTER->short_report(BUG_NEWSGROUP, TYPE, str, TYPE+" report");
#endif
    return 1;
}
