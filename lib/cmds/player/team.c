/* Do not remove the headers from this file! see /USAGE for more info. */

// team.c - Command to start up the team menu. Checks to see
//           if the player is currently in a team, and if not
//           prompts to see if the player wants to create a new
//           one. If the answer is no, the cmd quits without
//           creating a menu object.
// March 1, 1997: Iizuka@Lima Bean created.
// August 15, 1999: Iizuka@Lima Bean ripped out all of the modal code
//                  and moved it to TEAM_MENU.


//:PLAYERCOMMAND
//USAGE team
//
//Starts up the "team" menu system

inherit M_INPUT;
inherit CMD;

void main()
{
   new(TEAM_MENU)->join_team();
}
