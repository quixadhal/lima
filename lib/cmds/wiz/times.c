/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz

#include <mudlib.h>




#define USAGE	"Usage:  times <#> <command>\n"
int main( mixed * arg)
{
    int repeats;
    string	command;

    command = implode(arg[1]," ");
    repeats = arg[0];

    if( repeats <= 0 || ( repeats > 10 && !GROUP_D->adminp(this_user()) ) )
    {
	write( "times:  argument out of range.\n" );
	return 0;
    }

    while( repeats-- )
	this_user()->force_me( command );

    return 1;
}

