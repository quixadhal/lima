/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mailmsg.c -- class to hold a mail message
**
** 950910, Deathblade: Created.
*/

class mail_msg
{
    string *	to_list;	/* who msg was sent to */
    string *	cc_list;	/* who msg was copied to */
    string	sender;		/* who sent the msg */
    int		date;		/* when msg was sent */
    string	subject;	/* subject of msg */
    string *	body;		/* body of msg */
    string *	dels_pending;	/* these users must delete msg */
}
