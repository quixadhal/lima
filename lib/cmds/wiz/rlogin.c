/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** rlogin.c -- remote login (to another mud)
**
** This command is used to log into a remote mud to enable authenticated
** and privileged operations on that mud.
**
** 95-Apr-21.  Deathblade.  Created.
*/

#include <mudlib.h>

inherit CMD;
inherit M_REMOTE;
inherit M_INPUT;

static private string save_mud;
static private string * states = ({ "Not connected",
					"Connecting",
					"Connected" });
static private string * logged_in = ({ "", "Logged in" });


private void main(string arg)
{
  printf("This is a feature which doesn't work in the release right now.\n");
  return;

    if ( !arg || arg == "" )
    {
	write("RLOGIN STATUS\n" +
	      implode(map_array(muds_available(),
				(: sprintf("%10s: %-15s %-10s",
					   $1[0],
					   $(states)[$1[1]],
					   $(logged_in)[$1[2]]
					   ) :) ), "\n") +
	      "\n");

	return;
    }

    if ( is_logged_in(this_user(), arg) )
    {
	printf("You are already logged into %s.\n", arg);
	return;
    }

    clone_object(file_name(this_object()))->get_password(arg);
}

nomask void rcv_password(string password)
{
    if ( password == "" )
    {
	write("rlogin cancelled.\n");
    }
    else
    {
	login_remote(save_mud, password);
	write("rlogin request sent.\n");
    }
}

nomask void get_password(string mud)
{
    save_mud = mud;

    write("Password: ");
    modal_simple((: rcv_password :), 1);
}
