/* Do not remove the headers from this file! see /USAGE for more info. */

// _bug.c
// Rust wrote it
// Megaboz@ZorkMUD attached header 5-5-94
// Converted to use new /std/reporter superclass (Deathblade 4-Sep-94)
// Beek made one line reports work right 11/14/94
// Rust made them post news

#include <mudlib.h>
inherit CMD;

#define TYPE "Bug"

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
