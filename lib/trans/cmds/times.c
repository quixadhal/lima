/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// Repeats a command # of times
// Usage:  times <#> <command>

// Rust/Belboz

#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

#define USAGE	"Usage:  times <#> <command>\n"
private void main( string orig_input, mixed *arg )
{
    int repeats;
    string	command;


    command = implode(arg[1]," ");
    repeats = arg[0];

    if( repeats <= 0 || ( repeats > 10 && !check_privilege(1)) )
    {
	out( "times:  argument out of range.\n" );
	return;
    }

    while( repeats-- )
	this_user()->force_me( command );
}

