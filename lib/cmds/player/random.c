/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** random.c -- invoke a random emote
**
** 950826, Deathblade: created.
*/

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(string str)
{
    string * emotes = SOUL_D->list_emotes();
    string name = lower_case(trim_spaces(str));
    string rule;
    string extra;

    if ( find_body(name) )
    {
	rule = "LIV";
	extra = " " + name;
    }
    else
    {
	rule = extra = "";
    }

    while ( 1 )
    {
	string emote = emotes[random(sizeof(emotes))];

	if ( SOUL_D->query_emote(emote)[rule] )
	{
	    this_body()->do_game_command(emote + extra);
	    return;
	}
    }
}
