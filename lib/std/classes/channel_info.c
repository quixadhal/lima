/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** channel_info.c
**
** Define the "channel_info" class for objects that will need it.
*/


/*
** This class is used to store all the information related to a particular
** channel.
*/
class channel_info
{
    string	name;		/* name of the channel */

    object *	listeners;	/* who is listening */
    object *	hooked;		/* objects hooked into this channel */

    function	allowed;	/* is the user/ob allowed to listen? */

    object	moderator;	/* moderator for this channel */
    object	speaker;	/* the current speaker */
    object *	requestors;	/* who is in the queue to talk */

    mixed *	history;	/* channel history information */
}
