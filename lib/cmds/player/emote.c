/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_GRAMMAR;



private void main( string message )
{
    string name;

    if ( !message )
    {
	write("Emote what?\n");
	return;
    }

    name = this_body()->query_name();
    if (message[0] != '\'')
        message = punctuate(name + " " + message) + "\n";
    else
        message = punctuate(name + message) + "\n";

    write(iwrap("You emote: " + message));
    tell_room(environment(this_body()),
	      iwrap(message),
	      ({ this_body() }) );
}
