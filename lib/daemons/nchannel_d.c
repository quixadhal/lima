/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** channel_d.c
**
** Daemon for handling channels.
**
** Basic architecture: this daemon records all active channels for
** registered listener objects.  Incoming tells/emotes are then passed
** to the objects.  In particular, player objects register at login
** time and unregister at exit time.
**
** Note: (un)registration uses the previous object
**
** Client interface:
**	register_channels()	register a set of channels
**	unregister_channels()	unregister a specific set of channels
**	unregister_all()	unregister all channels
**
**	deliver_string()	deliver a raw string to listeners
**	deliver_channel()	deliver a string to a channel
**	deliver_tell()		deliver a (default-format) tell
**	deliver_emote()		deliver a (default-format) emote
**	deliver_soul()		deliver a (default-format) soul
**	deliver_notice()	deliver a (default-format) system notice
**
**	cmd_channel()		standard channel processing
**
** Listener interface:
**	channel_rcv_string()	receive a simple string
**	channel_rcv_soul()	receive a soul data array
**	channel_rcv_data()	receive data defining a channel use
**
** 05-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>
#include <security.h>

inherit M_ACCESS;
inherit M_GRAMMAR;


/*
** This maps channel names to an array of registered objects
*/
private static mapping registered;

/*
** These map channel names to a moderator, a speaker, and a queue of
** requestors.  The moderator and speaker have access to speak over the
** channel.
*/
private static mapping moderators;
private static mapping speakers;
private static mapping requestors;

/*
** Handy utility to create a "user name" from a real channel name
*/
private static function cl_user_chan_name;

/*
** Store some permanent listeners here
*/
#define SAVE_FILE	"/data/daemons/channel_d"

class listener_pair { string channel_name, filename; }
private class listener_pair *	saved_listeners;


private void register_one(string channel_name, object listener)
{
    if ( registered[channel_name] )
    {
	if ( member_array(listener, registered[channel_name]) == -1 )
	    registered[channel_name] += ({ listener });
    }
    else
	registered[channel_name] = ({ listener });
}

private void register_body(object body)
{
    string * names;

    if ( !(names = body->query_channel_list()) )
	return;

    map_array(names, (: register_one :), body);
}

private void unregister_one(string channel_name, object listener)
{
    registered[channel_name] -= ({ listener });
}

/*
** register_channels()
**
** Register previous_object() with a specific set of channels.
*/
nomask void register_channels(string * names)
{
    map_array(names, (: register_one :), previous_object());
}

/*
** unregister_channels()
**
** Un-register previous_object() from a specific set of channels.
*/
nomask void unregister_channels(string * names)
{
    map_array(names, (: unregister_one :), previous_object());
}

/*
** unregister_all()
**
** Un-register all the channels that previous_object() is tuned into.
*/
nomask void unregister_all()
{
    map_array(keys(registered), (: unregister_one :), previous_object());
}

/*
** find_sender_name()
**
** Find the sender's name
*/
private nomask string find_sender_name(string sender_name)
{
    if ( sender_name )
	return sender_name;

    if ( this_body() )
	if ( sender_name = this_body()->query_name() )
	    return sender_name;

    if ( !(sender_name = previous_object()->query_name()) )
	sender_name = "<unknown>";

    return sender_name;
}

/*
** deliver_string()
**
** Deliver a raw string over a channel.
*/
nomask void deliver_string(string channel_name, string str)
{
    object * listeners;

    if ( !(listeners = registered[channel_name]) ||
	sizeof(listeners) == 0 )
	return;

    listeners->channel_rcv_string(channel_name, str);
}

/*
** deliver_channel()
**
** Deliver a string to a channel, prepending the channel name
*/
nomask void deliver_channel(string channel_name, string str)
{
    object * listeners;

    if ( !(listeners = registered[channel_name]) ||
	sizeof(listeners) == 0 )
	return;

    str = iwrap(sprintf("[%s] %s\n",
			evaluate(cl_user_chan_name, channel_name),
			str));
    listeners->channel_rcv_string(channel_name, str);
}

/*
** deliver_raw_soul()
**
** Deliver raw "soul" data over a channel.
*/
nomask void deliver_raw_soul(string channel_name, mixed * data)
{
    object * listeners;

    if ( !(listeners = registered[channel_name]) ||
	sizeof(listeners) == 0 )
	return;

    listeners->channel_rcv_soul(channel_name, data);
}

/*
** deliver_data()
**
** Deliver unformatted channel data to the listeners
*/
private nomask void deliver_data(string channel_name,
				 string sender_name,
				 string type,
				 mixed data)
{
    object * listeners;

    if ( !(listeners = registered[channel_name]) ||
	sizeof(listeners) == 0 )
	return;

    listeners->channel_rcv_data(channel_name, sender_name, type, data);
}

/*
** deliver_tell()
**
** Deliver a standard-formatted "tell" over a channel
*/
nomask void deliver_tell(string channel_name,
			 string sender_name,
			 string message)
{
    sender_name = find_sender_name(sender_name);

    deliver_data(channel_name, sender_name, "tell", message);
    deliver_string(channel_name,
		   iwrap(sprintf("[%s] %s: %s\n",
				 evaluate(cl_user_chan_name, channel_name),
				 sender_name,
				 punctuate(message))));
}

/*
** deliver_emote()
**
** Deliver a standard-formatted "emote" over a channel
*/
nomask void deliver_emote(string channel_name,
			  string sender_name,
			  string message)
{
    sender_name = find_sender_name(sender_name);

    deliver_data(channel_name, sender_name, "emote", message);
    deliver_string(channel_name,
		   iwrap(sprintf("[%s] %s %s\n",
				 evaluate(cl_user_chan_name, channel_name),
				 sender_name,
				 punctuate(message))));
}

/*
** deliver_soul()
**
** Deliver a standard-formatted "soul" over a channel
*/
nomask void deliver_soul(string channel_name, mixed * soul)
{
    string user_channel_name;

    deliver_data(channel_name, find_sender_name(0), "soul", soul);

    user_channel_name = evaluate(cl_user_chan_name, channel_name);
    soul = ({ soul[0] }) +
	({ map_array(soul[1],
		     (: iwrap(sprintf($("["+user_channel_name+"] %s"), $1)) :))
       });

    deliver_raw_soul(channel_name, soul);
}

/*
** deliver_notice()
**
** Deliver a standard-formatted "system notice" over a channel
*/
nomask void deliver_notice(string channel_name,
			   string message)
{
    deliver_string(channel_name,
		   iwrap(sprintf("[%s] (%s)\n",
				 evaluate(cl_user_chan_name, channel_name),
				 message)));
}

/*
** create()
**
** duh.
*/
void create()
{
    set_privilege(1);

    registered = ([ ]);
    moderators = ([ ]);
    speakers   = ([ ]);
    requestors = ([ ]);

    cl_user_chan_name = (: explode($1, "_")[<1] :);

    map_array(all_users()->query_body(), (: register_body :));

    restore_object(SAVE_FILE, 1);
    if ( saved_listeners )
    {
	class listener_pair pair;

	foreach ( pair in saved_listeners )
	{
	    object ob = find_object(pair->filename);

	    if ( ob )
		register_one(pair->channel_name, ob);
	}

	saved_listeners = 0;
    }
}

/*
** remove()
**
** Write out all listeners that are blueprints.  We'll reset them at
** creation time.
*/
void remove()
{
    string channel_name;
    object * listeners;

    saved_listeners = ({ });

    foreach ( channel_name, listeners in registered )
    {
	object listener;

	foreach ( listener in listeners - ({ 0 }) )
	{
	    string fname = file_name(listener);

	    if ( member_array('#', fname) == -1 )
	    {
		class listener_pair pair = new(class listener_pair);

		pair->channel_name = channel_name;
		pair->filename = fname;
		saved_listeners += ({ pair });
	    }
	}
    }

    unguarded(1, (: save_object, SAVE_FILE :));
}

/*
** query_register()
**
** for debugging and other maintainance
*/
nomask mapping query_registered()
{
    if ( check_privilege(1))
	return registered;
}

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
    object moderator;
    
    moderator = moderators[channel_name];
    if ( !moderator )
	return;

    printf("It is being moderated by %s.\n", moderator->query_name());

    if ( speakers[channel_name] )
	printf("The current speaker is %s.\n",
	       speakers[channel_name]->query_name());
    else
	printf("There is no current speaker.\n");

    if ( moderator == this_body() )
    {
	if ( !requestors[channel_name] ||
	    !sizeof(requestors[channel_name]) )
	    printf("There are no requestors.\n");
	else
	    write(iwrap(sprintf("Requestors are: %s.\n",
				make_name_list(requestors[channel_name]))));
    }
    else if ( member_array(this_body(), requestors[channel_name]) != -1 )
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
    object tb;
    string * channel_list;
    int listening;
    string user_channel_name;
    object moderator;
    string sender_name;

    tb = this_body();
    channel_list = tb->query_channel_list();
    listening = member_array(channel_name, channel_list) != -1;
    user_channel_name = evaluate(cl_user_chan_name, channel_name);
    moderator = moderators[channel_name];
			       
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
	if ( tb == moderator )
	{
	    map_delete(moderators, channel_name);
	    map_delete(speakers, channel_name);
	    map_delete(requestors, channel_name);

	    deliver_notice(channel_name, "This channel is now unmoderated");
	}
	else if ( tb == speakers[channel_name] )
	{
	    map_delete(speakers, channel_name);
	    deliver_notice(channel_name,
			   sprintf("%s is no longer speaking", sender_name));
	}
    }
    else if ( arg == "/list" )
    {
	write(iwrap(sprintf("Users listening to '%s': %s.\n",
			    user_channel_name,
			    make_name_list(registered[channel_name]))));
    }
    else if ( arg == "/raise" )
    {
	if ( !moderator )
	{
	    printf("'%s' is not moderated.\n", user_channel_name);
	}
	else if ( tb == speakers[channel_name] )
	{
	    printf("You are already speaking on '%s'.\n", user_channel_name);
	}
	else if ( member_array(tb, requestors[channel_name]) == -1 )
	{
	    printf("Your raise your hand to speak on '%s'.\n",
		   user_channel_name);
	    requestors[channel_name] += ({ tb });
	    moderator->channel_rcv_string(channel_name,
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
	if ( !moderator )
	{
	    printf("'%s' is not moderated.\n", user_channel_name);
	}
	else if ( member_array(tb, requestors[channel_name]) != -1 )
	{
	    printf("Your lower your hand to avoid speaking on '%s'.\n",
		   user_channel_name);
	    requestors[channel_name] -= ({ tb });
	    moderator->channel_rcv_string(channel_name,
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
	if ( !moderator )
	{
	    printf("'%s' is not moderated.\n", user_channel_name);
	}
	else if ( moderator != tb )
	{
	    printf("You are not the moderator of '%s'.\n", user_channel_name);
	}
	else if ( arg == "" )
	{
	    if ( sizeof(requestors[channel_name]) == 0 )
		printf("Nobody has their hand raised.\n");
	    else
	    {
		speakers[channel_name] = requestors[channel_name][0];
		requestors[channel_name] = requestors[channel_name][1..];
		deliver_notice(channel_name,
			       sprintf("%s will now speak",
				       speakers[channel_name]->query_name()));
	    }
	}
	else
	{
	    object * spkr;

	    spkr = filter_array(requestors[channel_name],
				(: $(arg) == lower_case($1->query_name()) :));
	    if ( sizeof(spkr) == 0 )
	    {
		printf("'%s' was not found (or did not have their hand raised.\n",
		       capitalize(arg));
	    }
	    else
	    {
		speakers[channel_name] = spkr[0];
		requestors[channel_name] -= ({ spkr[0] });
		deliver_notice(channel_name,
			       sprintf("%s will now speak",
				       speakers[channel_name]->query_name()));
	    }
	}
    }
    else if ( arg == "/moderate" )
    {
	if ( GROUP_D->adminp(this_user()) ||
	    GROUP_D->member_group(this_user()->query_real_name(), "moderators") )
	{
	    moderators[channel_name] = tb;
	    if ( !requestors[channel_name] )
		requestors[channel_name] = ({ });
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
        if ( moderator && speakers[channel_name] &&
	    tb != moderator &&
	    tb != speakers[channel_name] )
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
        if (moderator && speakers[channel_name] &&
	    tb != moderator &&
	    tb != speakers[channel_name] )
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
        if (moderator && speakers[channel_name] &&
	    tb != moderator &&
	    tb != speakers[channel_name] )
	{
	    printf("You are not the speaker on '%s'.\n", user_channel_name);
	}
	else
	{
	    deliver_tell(channel_name, sender_name, arg);
	}
    }
}
