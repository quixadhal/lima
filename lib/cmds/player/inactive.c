/* Do not remove the headers from this file! see /USAGE for more info. */

// Jan 10, 94 by Rust
// Megaboz@ZorkMUD added help

#include <mudlib.h>
#include <playerflags.h>


inherit M_INPUT;


private nomask void become_active(int start_time, string str)
{
    int num;
    int hours, minutes;

    num = time() - start_time;
    if(num>359)
	hours = num/3600;
    num -= hours*3600;
    if(num>59)
	minutes = num/60;
    num -= minutes*60;
    write("You return to ACTIVE status after ");
    if(hours) printf("%d hours, ",hours);
    if(minutes) printf("%d minutes, ",minutes);
    printf("%d seconds.\n",num);
    say(this_body()->query_name() + " returns to ACTIVE status.\n");
    this_body()->clear_flag(F_INACTIVE);
}


nomask int main(string arg)
{
    write("You place yourself in INACTIVE mode.  Press <ENTER> to return.\n");
    say(this_body()->query_name()+" goes into INACTIVE mode.\n");
    this_body()->set_flag(F_INACTIVE);
    modal_simple((: become_active, time() :));
    return 1;
}


nomask int help()
{
    printf("USAGE:  inactive\n"+
      wrap("This is the best way for you to let people that you are away from your keyboard.  You just have to hit the enter key when you return, and you will leave off where you left off, unless you, of course, exceed the limit for idling.  In which case, you won't.  =->")+"\n");
    return 1;
}
