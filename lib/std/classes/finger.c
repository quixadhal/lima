/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** finger.c -- specifies the class that holds "finger" info
**
** This is built by the FINGER_D and consumed by the finger command, the
** WWW finger CGI, and intermud "who"
**
** Note that fields within the class may be zeroed if the information is
** not available based on the mud's configuration, or based upon the
** caller's security.
*/

class finger
{
    string	visname;
    string	nickname;
    string	title;
    string	position;
    string	real_name;
    string	email;
    string	home_page;
    int		last_login;
    int		idle;
    string	connect_from;
    string	level;		/* e.g. "player", "wizard", "admin" */
    string	plan;
    int		mail_count;
    int		mail_unread;
}
