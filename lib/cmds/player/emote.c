/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_GRAMMAR;



int main( string message )
{
    string name;
    name = this_body()->query_name();
    if (message[0] != '\'')
        message = punctuate(name + " " + message) + "\n";
    else
        message = punctuate(name + message) + "\n";

    write(iwrap("You emote: " + message));
    tell_room(environment(this_body()),
	      iwrap(message),
	      ({ this_body() }) );

    return 1;
}
