/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** channel/cmd.c
**
** Part of the CHANNEL_D to handle channel command processing.
*/

#include <classes.h>

inherit CLASS_CHANNEL_INFO;

class channel_info query_channel_info(string channel_name);
string user_channel_name(string channel_name);
void deliver_notice(string channel_name, string message);
void deliver_emote(string channel_name,
		   string sender_name,
		   string message);
void deliver_tell(string channel_name,
		  string sender_name,
		  string message);
void deliver_soul(string channel_name, mixed * soul);


/*
** make_name_list()
**
** Make a list of names given an array of players.
*/
private nomask string make_name_list(mixed * list)
{
    /*
    ** Remove null objects, objects with no links (to interactive users),
    ** and link obs that are no longer interactive.
    */
    list = filter_array(list, (: $1 && $1->query_link() &&
			       interactive($1->query_link()) :));
    return implode(list->query_name(), ", ");
}

    
/*
** print_mod_info()
**
** Print moderator/speak infor for a moderated channel.
*/
private nomask void print_mod_info(string channel_name)
{
    class channel_info ci = query_channel_info(channel_name);

    if ( !ci->moderator )
	return;

    printf("It is being moderated by %s.\n", ci->moderator->query_name());

    if ( ci->speaker )
	printf("The current speaker is %s.\n", ci->speaker->query_name());
    else
	printf("There is no current speaker.\n");

    if ( ci->moderator == this_body() )
    {
	if ( !ci->requestors ||
	    !sizeof(ci->requestors) )
	    printf("There are no requestors.\n");
	else
	    write(iwrap(sprintf("Requestors are: %s.\n",
				make_name_list(ci->requestors))));
    }
    else if ( member_array(this_body(), ci->requestors) != -1 )
    {
	printf("Your hand is raised to speak.\n");
    }
}

/*
** cmd_channel()
**
** Standard channel processing command for player input.  Most channel-
** oriented systems will use this to get standardized channel manipulation.
**
** channel_type is:
**	0 normal
**	1 intermud
*/
varargs nomask void cmd_channel(string channel_name, string arg,
				int channel_type)
{
    class channel_info ci = query_channel_info(channel_name);
    object tb;
    string * channel_list;
    int listening;
    string user_channel_name;
    string sender_name;

    tb = this_body();
    channel_list = tb->query_channel_list();
    listening = member_array(channel_name, channel_list) != -1;
    user_channel_name = user_channel_name(channel_name);

    if ( !arg || arg == "" )
    {
	if ( listening )
	{
	    printf("You are presently listening to '%s'.\n", user_channel_name);
	    print_mod_info(channel_name);
	}
	else
	{
	    printf("You are not listening to '%s'.\n", user_channel_name);
	}

	return;
    }

    if ( arg == "/on" )
    {
	if ( listening )
	    printf("You are already listening to '%s'.\n", user_channel_name);
	else
	{
	    tb->channel_add(channel_name);
	    printf("You are now listening to '%s'.\n", user_channel_name);
	}

	print_mod_info(channel_name);

	return;
    }

    /*
    ** All the following "commands" need to have the player listening
    */
    if ( !listening )
    {
	printf("You are not listening to '%s'.\n", user_channel_name);

	return;
    }
    
    sender_name = tb->query_name();

    if ( arg == "/off" )
    {
	tb->channel_remove(channel_name);
	printf("You are no longer listening to '%s'.\n", user_channel_name);
	if ( tb == ci->moderator )
	{
	    ci->moderator =
		ci->speaker =
		ci->requestors = 0;

	    deliver_notice(channel_name, "This channel is now unmoderated");
	}
	else if ( tb == ci->speaker )
	{
	    ci->speaker = 0;
	    deliver_notice(channel_name,
			   sprintf("%s is no longer speaking", sender_name));
	}
    }
    else if ( arg == "/list" )
    {
	write(iwrap(sprintf("Users listening to '%s': %s.\n",
			    user_channel_name,
			    make_name_list(ci->listeners))));
    }
    else if ( arg == "/raise" )
    {
	if ( !ci->moderator )
	{
	    printf("'%s' is not moderated.\n", user_channel_name);
	}
	else if ( tb == ci->speaker )
	{
	    printf("You are already speaking on '%s'.\n", user_channel_name);
	}
	else if ( member_array(tb, ci->requestors) == -1 )
	{
	    printf("Your raise your hand to speak on '%s'.\n",
		   user_channel_name);
	    ci->requestors += ({ tb });
	    ci->moderator->channel_rcv_string(channel_name,
					  sprintf("[%s] (%s raises a hand to speak)\n",
						  user_channel_name,
						  sender_name));
	}
	else
	{
	    printf("You already have your hand raised to speak on '%s'.\n",
		   user_channel_name);
	}
    }
    else if ( arg == "/lower" )
    {
	if ( !ci->moderator )
	{
	    printf("'%s' is not moderated.\n", user_channel_name);
	}
	else if ( member_array(tb, ci->requestors) != -1 )
	{
	    printf("Your lower your hand to avoid speaking on '%s'.\n",
		   user_channel_name);
	    ci->requestors -= ({ tb });
	    ci->moderator->channel_rcv_string(channel_name,
					  sprintf("[%s] (%s lowers a hand)\n",
						  user_channel_name,
						  sender_name));
	}
	else
	{
	    printf("Your hand is not raised to speak on '%s'.\n",
		   user_channel_name);
	}
    }
    else if ( arg[0..3] == "/call" )
    {
	arg = lower_case(trim_spaces(arg[4..]));
	if ( !ci->moderator )
	{
	    printf("'%s' is not moderated.\n", user_channel_name);
	}
	else if ( ci->moderator != tb )
	{
	    printf("You are not the moderator of '%s'.\n", user_channel_name);
	}
	else if ( arg == "" )
	{
	    if ( sizeof(ci->requestors) == 0 )
		printf("Nobody has their hand raised.\n");
	    else
	    {
		ci->speaker = ci->requestors[0];
		ci->requestors = ci->requestors[1..];
		deliver_notice(channel_name,
			       sprintf("%s will now speak",
				       ci->speaker->query_name()));
	    }
	}
	else
	{
	    object * spkr;

	    spkr = filter_array(ci->requestors,
				(: $(arg) == lower_case($1->query_name()) :));
	    if ( sizeof(spkr) == 0 )
	    {
		printf("'%s' was not found (or did not have their hand raised.\n",
		       capitalize(arg));
	    }
	    else
	    {
		ci->speaker = spkr[0];
		ci->requestors -= ({ spkr[0] });
		deliver_notice(channel_name,
			       sprintf("%s will now speak",
				       ci->speaker->query_name()));
	    }
	}
    }
    else if ( arg == "/moderate" )
    {
	if ( adminp(this_user()) ||
	    GROUP_D->member_group(this_user()->query_userid(), "moderators") )
	{
	    ci->moderator = tb;
	    if ( !ci->requestors )
		ci->requestors = ({ });
	    deliver_notice(channel_name,
			   sprintf("%s is now moderating", sender_name));
	}
	else
	{
	    printf("You are not allowed to moderate this channel.\n");
	}
    }
    else if ( arg[0] == ':' )
    {
        if ( ci->moderator && ci->speaker &&
	    tb != ci->moderator && tb != ci->speaker )
	{
	    printf("You are not the speaker on '%s'.\n", user_channel_name);
	}
	else
	{
	    deliver_emote(channel_name, sender_name, arg[1..]);
	}
    }
    else if ( arg[0] == ';' )
    {
        if ( ci->moderator && ci->speaker &&
	     tb != ci->moderator && tb != ci->speaker )
	{
	    printf("You are not the speaker on '%s'.\n", user_channel_name);
	}
	else if ( channel_type == 1 )
	{
	    mixed * soul;

	    soul = SOUL_D->parse_imud_soul(arg[1..]);
	    if ( soul )
		deliver_soul(channel_name, soul);
	    else
		deliver_emote(channel_name, sender_name, arg[1..]);
	}
	else
	{
	    mixed * soul;

	    soul = SOUL_D->parse_soul(arg[1..]);
	    if ( soul )
		deliver_soul(channel_name, soul);
	    else
		deliver_emote(channel_name, sender_name, arg[1..]);
	}
    }
    else
    {
        if ( ci->moderator && ci->speaker &&
	     tb != ci->moderator && tb != ci->speaker )
	{
	    printf("You are not the speaker on '%s'.\n", user_channel_name);
	}
	else
	{
	    deliver_tell(channel_name, sender_name, arg);
	}
    }
}
