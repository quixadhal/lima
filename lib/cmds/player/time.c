/* Do not remove the headers from this file! see /USAGE for more info. */

/********************************************************
 *                                                      *
 *   uptime  -  reports how long the mud has been up.   *

 ** *  copied from ideaexchange where it was originally*
 *  created by beek@nightmare                     *

 *   copied by zifnab@zorkmud                           *
 *
 *********************************************************/
#include <mudlib.h>
#include <daemons.h>

inherit CMD;

#define MIN 60
#define HOUR (60 * MIN)
#define DAY (24 * HOUR)
#define WEEK (7 * DAY)


private void main(string notused)
{
    int tm = uptime();
    string tm2 = ctime(time());
    int tm3 = time() - uptime();
    string tm4 = ctime(tm3);
    string str;
 
    int x;
   out("Local MUD time       "+ tm2 +". \n");
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

    {
	str += tm + "s ";
    }

    str = str[0..<2] + ".\n";
    out(str);
}
