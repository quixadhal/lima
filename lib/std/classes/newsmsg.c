/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** newsmsg.c -- class to hold a news message
**
** 950811, Deathblade: Created.
*/

class news_msg
{
    mixed	time;		/* time() or a string */
    int		thread_id;	/* id of first post on thread */
    string	subject;
    string	poster;		/* visname of poster */
    string	userid;		/* userid of poster */
    string	body;		/* 0 when removed */
}
