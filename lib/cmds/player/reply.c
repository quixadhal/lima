/* Do not remove the headers from this file! see /USAGE for more info. */

// This space intentionally left blank
// Megaboz@ZorkMUD attached header and help

#include <commands.h>
#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string arg)
{
    string   target;
    string	tmp, tmp2;

    if(!arg || arg == "")
      {
	out("Usage: reply <message>\n");
	return;
      }
    target = this_body()->query_reply();

    if( !find_user( target ) && !sscanf( target, "%s@%s", tmp, tmp2 ) )
    { 
	out( "No target found for reply.\n" );
	return;
    }

    resend(CMD_OB_TELL, sprintf("%s %s", target, arg));
}
