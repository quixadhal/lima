/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mail.h -- various items needed for mail handling
**
** 950701, Deathblade: Created.
*/

#ifndef __MAIL_H__
#define __MAIL_H__

/*
** This class defines the contents of a mail message
*/
class mail_msg
{
    string *	to_list;	/* who msg was sent to */
    string *	cc_list;	/* who msg was copied to */
    string	from;		/* who sent the msg */
    int		date;		/* when msg was sent */
    string	subject;	/* subject of msg */
    string *	body;		/* body of msg */
    string *	dels_pending;	/* these users must delete msg */
}


#endif /* __MAIL_H__ */
