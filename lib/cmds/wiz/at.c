/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

#define USAGE "Usage: at <living> <command>\n"
int main( mixed *arg ) {
    string where;
    object start, targ;

    start = environment(this_body());
    targ = environment(arg[0]);
    if(!targ){
	write("Your target has no environment.\n");
	return 0;
    }
    this_body()->move( targ );
    this_user()->force_me(implode(arg[1], " "));
    if(!start) return 1;
    this_body()->move( start );
    return 1;
}
