/* Do not remove the headers from this file! see /USAGE for more info. */

// _date.c
// written by ???
// Megaboz@ZorkMUD added help and header 5-5-94

#include <mudlib.h>
inherit CMD;

private void main(){
//add TZONE conversions
write (ctime(time())+"\n");
return;
}

int help()
{
    printf("Usage:  date\nArguments:  none\n\n"+
	   "Displays the current date and time.\n"+
	   "To display the date with your current time,\n"+
	   "use the 'timezone' command.\n");
    printf("The timezone command is not currently available.\n");
    return 1;
}
