/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_GRAMMAR;


void create()
{
  ::create();
  no_redirection();
}

private void main(string message, mapping flags, string stdin, mixed impl)
{
    string name;

    if ( !message )
    {
        if(stdin)
	  message = stdin;
	else
	  {
	    write("Emote what?\n");
	    return;
	  }
    }

    name = this_body()->query_name();
    if (message[0] != '\'')
        message = punctuate(name + " " + message) + "\n";
    else
        message = punctuate(name + message) + "\n";

    out(iwrap("You emote: " + message));
    tell_room(environment(this_body()),
	      iwrap(message), 0,
	      ({ this_body() }) );
}
