/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** title.c -- title handling for the player
**
** Designed to be removed from the player object if necessary.
**
** 95-May-01. Deathblade. Created.
*/

private string title;

string query_name();		/* in /std/player */

string query_title()
{
    /* ensure the player has a title. set it if none (yet) */
    if ( !title )
	title = sprintf("%s the title-less", capitalize(query_name()));

    return title;
}

int set_title(string str)
{
    if ( this_body() != this_object() )
	error("invalid attempt to set a title\n");

    if ( str && strsrch(str, "$N") == -1 )
	error("bad title -- needs to contain $N\n");

    if ( !str )
	title = sprintf("%s the title-less.", capitalize(query_name()));
    else
	title = replace_string(str,"$N", capitalize(query_name()));
    title += "%^RESET%^";
}
