/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: didlog
//USAGE:  I <string>
//
//This command produces the "did" log when you first log on.
//The use of thes command allows you to let other wizards
//know about any changes or additions that you made to the mud.
//
//I started adding help files for some wiz cmds
//
//When logging in next time you (and other wizards) will see:
//
//Wed Aug 23 17:48:49 1995: Zifnab started adding help files for some wiz
//cmds

#include <daemons.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string str) {
    if (!str || str == "") {
         out("I <whatever you did>\n");
         return;
    }
   if( DID_D->someone_did(str))
    out("Reported.  (Thank you!)\n");
}

