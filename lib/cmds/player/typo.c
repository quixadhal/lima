/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** typo.c
**
** Converted to use new /std/reporter superclass (Deathblade 4-Sep-94)
** Original by Rust 
** Rust made it post news.
*/

#include <mudlib.h>

#define LOGFILE "TYPOS"
#define TYPE "Typo"

int main(string str)
{
#ifdef LOG_DONT_POST
  if(!str || str == "")
    REPORTER->begin_report(LOGFILE, TYPE);
  else
    REPORTER->short_report(LOGFILE, TYPE, str);
#else
    if (!str || str == "")
        REPORTER->begin_report(TYPO_NEWSGROUP, TYPE, TYPE+" report");
    else
         REPORTER->short_report(TYPO_NEWSGROUP, TYPE, str, TYPE+" report");
#endif
    return 1;
}
