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
string query_userid();		/* in /std/player */
string query_idle_string();     /* in /std/player */
string base_in_room_desc();     /* in /std/player */

string query_title()
{
    return in_room_desc();
}

string query_truncated_title(int num_chars)
{
  string idle_string;
  int i;
  
  idle_string = query_idle_string();
  if(i=strlen(idle_string))
    {
      num_chars -= (i+1);
      idle_string = " " + idle_string;
    }
  return truncate(base_in_room_desc(), num_chars) +  idle_string;
}

int set_title(string str)
{
    if(this_body() != this_object())
      return;

    if ( str && strsrch(str, "$N") == -1 )
	return -1;

    if ( !str )
	title = sprintf("%s the title-less.", capitalize(query_userid()));
    else
	title = replace_string(str,"$N", capitalize(query_userid()));
}
