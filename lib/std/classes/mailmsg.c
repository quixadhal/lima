/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mailmsg.c -- class to hold a mail message
**
** 950910, Deathblade: Created.
*/

class mail_msg
{
  // IMUD_D wants mappings, MAIL_D wants string arrays for 
  // both of these.
    mixed	to_list;	/* who msg was sent to */
    mixed	cc_list;	/* who msg was copied to */
    string	sender;		/* who sent the msg */
    int		date;		/* when msg was sent */
    string	subject;	/* subject of msg */
    int		thread_id;	/* first msg's id (date) */
    string *	body;		/* body of msg */
  // IMUD_MAIL doesn't need to know WHO hasn't deleted the message, so
  // it just uses a number.  regular mail uses an array of names.
    mixed	dels_pending;	/* these users must delete msg */
}
