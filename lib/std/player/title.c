/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** title.c -- title handling for the player
**
** Designed to be removed from the player object if necessary.
**
** 95-May-01. Deathblade. Created.
*/

string title;
private string invis_title;

string in_room_desc();		/* in /std/player */
object query_link();		/* in /std/player */
string query_real_name();	/* in /std/player */


string query_title()
{
    return in_room_desc();
}

int set_title(string str)
{
    string foo;

    if(this_body() != this_object())
      return;

    if ( str && !sscanf(str,"%s$N%s", foo, foo) )
	return -1;

    if ( !str )
	title = sprintf("%s the title-less.", capitalize(query_real_name()));
    else
	title = replace_string(str,"$N", capitalize(query_real_name()));

    query_link()->set("title",title);
}
