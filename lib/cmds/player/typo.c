/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** typo.c
**
** Converted to use new /std/reporter superclass (Deathblade 4-Sep-94)
** Original by Rust 
** Rust made it post news.
*/

#include <mudlib.h>

inherit CMD;

#define TYPE "Typo"

private void main(string str)
{
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
