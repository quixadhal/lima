/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** _idea.c
**
** Converted to new /std/reporter usage (Deathblade 4-Sep-94).
** Original by Rust (?)
*/

#include <mudlib.h>
inherit CMD;

#define TYPE "Idea"

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

  return;
}
