/* Do not remove the headers from this file! see /USAGE for more info. */

// specification is: snoop [user] [str*]
// we will recieve: ({ user, ({ words }) })

#include <mudlib.h>
#include <log.h>

inherit CMD;

private void main(mixed *arg)
{
  int rc;
  if ( !arg[0] )
    {
      rc = SNOOP_D->unsnoop_all(this_body());
      out(rc ? "Snoop someone first, dumbass.\n" : "Okay.\n");
      return;
    }
  else
    {
      rc = SNOOP_D->unsnoop(this_body(), arg[0]);
      out(rc ? "Like you're snooping 'em, duh!\n" : "Okay.\n");
      return;
    }
}
