/* Do not remove the headers from this file! see /USAGE for more info. */

/********************************************************
 *                                                      *
 *   uptime  -  reports how long the mud has been up.   *

 ** *  copied from ideaexchange where it was originally*
 *  created by beek@nightmare                     *

 *   copied by zifnab@zorkmud                           *
 *
 *********************************************************/

//:PLAYERCOMMAND
//$$ see: timezone
//USAGE time
//
//Displays the current local time (where the mud server is located),
//and the GMT conversion of that, and player's local time (if the
//GMT offset has been entered using the "timezone" command)
//together with time the mud was last rebooted (on mud local time),
//and how long it has been up.

#include <localtime.h>

inherit CMD;

#define MIN 60
#define HOUR (60 * MIN)
#define DAY (24 * HOUR)
#define WEEK (7 * DAY)

private void main(string notused)
{
  int tm = uptime();
  int tm1 = time();
  string tm2 = ctime(tm1);
  int tm3 = tm1 - uptime();
  string tm4 = ctime(tm3);
  int tm5;
  string str;
  mixed local = localtime(tm1);
  int offset = local[LT_GMTOFF];
  string gmt;
  int x;
  mixed my_offset;

  my_offset = this_body()->query_tz();
  if(intp(my_offset) || floatp(my_offset))
  {
    my_offset = to_int(3600.0*my_offset);
    tm5 = tm1+my_offset;
  }
  gmt = ctime(tm1-offset);
  outf("Local MUD time %s. \n", tm2);
  outf("GMT            %s.\n", gmt);
  my_offset = this_body()->query_tz();
  if(intp(my_offset) || floatp(my_offset))
  {
    my_offset = to_int(3600.0*my_offset);
    tm5 = tm1+my_offset;
    outf("Player time    %s.\n", ctime(tm1+my_offset));
  }
  out(mud_name()+" restarted on " + tm4 +". \n \n");
  str = mud_name()+ " has been up for ";
  if (x = (tm / WEEK))
  {
    str += x + "w ";
    tm -= x * WEEK;
  }

  if (x = (tm / DAY))
  {
    str += x + "d ";
    tm -= x*DAY;
  }

  if (x = (tm / HOUR))
  {
    str += x + "h ";
    tm -= x * HOUR;
  }

  if (x = (tm / MIN))
  {
    str += x + "m ";
    tm -= x* MIN;
  }

  if (tm)
    str += tm + "s ";

  str = str[0..<2] + ".\n";
  out(str);
}
