/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** didlog.c -- print out a portion of the DID_D log
**
** 950821, Deathblade: created
*/

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(string str) {
    int ndays;
    string header;
    string pattern;

    if ( !str )
	ndays = 1;
    else if ( str == "/off" && this_body() )
    {
      this_body()->set_ilog_off(1);
      out("You will no longer receive didlog notifications.\n");
      return;
    }
    else if ( str == "/on" && this_body() )
    {
      this_body()->set_ilog_off(0);
      out("You will now receive didlog notifications.\n");
      return;
    }
    else if ( !(ndays = to_int(str)) ) {
	if (sscanf(str, "%s %d", pattern, ndays) != 2) {
	    pattern = str;
	    ndays = 1;
	}
    }

    if ( ndays == 1 )
	header = "DID_D report for the past day";
    else
	header = sprintf("DID_D report for the past %d days", ndays);

    out(DID_D->get_did_info(time() - ndays * 24 * 60 * 60,
	({ header,
	  repeat_string("-", sizeof(header)),
	  "" }),
	pattern));
}

