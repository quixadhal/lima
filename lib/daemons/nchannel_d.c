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
#include <classes.h>

inherit M_ACCESS;
inherit M_GRAMMAR;

//inherit CLASS_CHANNEL_INFO;   picked up from channel/cmd

inherit __DIR__ "channel/cmd";


/*
** This channel information.  It specifies channel_name -> channel_info.
*/
private static mapping info;


/*
** Store some permanent listeners here
*/
#define SAVE_FILE	"/data/daemons/channel_d"

class listener_pair
{
    string channel_name;
    string filename;
}
private class listener_pair *	saved_listeners;
private class listener_pair *	saved_hooks;


private nomask string extract_channel_name(string channel_name)
{
    int idx = strsrch(channel_name, "_", 1);

    if ( idx == -1 )
	return channel_name;

    return channel_name[idx+1..];
}

static nomask class channel_info query_channel_info(string channel_name)
{
    return info[channel_name];
}

private void register_one(int add_hook, object listener, string channel_name)
{
    class channel_info	ci = info[channel_name];

    if ( !ci )
    {
	ci = new(class channel_info);
	ci->name	= extract_channel_name(channel_name);
	ci->listeners	= ({ });
	ci->hooked	= ({ });
	ci->allowed	= (function)1;

	info[channel_name] = ci;
    }

    if ( add_hook )
    {
	if ( member_array(listener, ci->hooked) == -1 )
	    ci->hooked += ({ listener });
    }
    else
    {
	if ( member_array(listener, ci->listeners) == -1 )
	    ci->listeners += ({ listener });
    }
}

private void unregister_one(string channel_name, object listener)
{
    class channel_info ci = info[channel_name];

    if ( ci )
    {
	ci->listeners -= ({ listener });
	if ( sizeof(ci->listeners) + sizeof(ci->hooked) == 0 )
	    map_delete(info, channel_name);
    }
}

private void register_body(object body)
{
    string * names;

    if ( !(names = body->query_channel_list()) )
	return;

    map_array(names, (: register_one, 0, body :));
}

/*
** user_channel_name()
**
** Return a channel name that a user might like.  Internal names are
** not "human readable"
*/
nomask string user_channel_name(string channel_name)
{
    class channel_info	ci = info[channel_name];

    if ( ci )
	return ci->name;

    return extract_channel_name(channel_name);
}

/*
** register_channels()
**
** Register previous_object() with a specific set of channels.
*/
nomask void register_channels(string * names)
{
    map_array(names, (: register_one, 0, previous_object() :));
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
    map_array(keys(info), (: unregister_one :), previous_object());
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
    class channel_info ci = info[channel_name];

    if ( !ci ||	sizeof(ci->listeners) == 0 )
	return;

    ci->listeners->channel_rcv_string(channel_name, str);
}

/*
** deliver_channel()
**
** Deliver a string to a channel, prepending the channel name
*/
nomask void deliver_channel(string channel_name, string str)
{
    deliver_string(channel_name,
		   iwrap(sprintf("[%s] %s\n",
				 user_channel_name(channel_name),
				 str)));
}

/*
** deliver_raw_soul()
**
** Deliver raw "soul" data over a channel.
*/
nomask void deliver_raw_soul(string channel_name, mixed * data)
{
    class channel_info ci = info[channel_name];

    if ( !ci ||	sizeof(ci->listeners) == 0 )
	return;

    ci->listeners->channel_rcv_soul(channel_name, data);
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
    class channel_info ci = info[channel_name];

    if ( !ci ||	sizeof(ci->listeners) == 0 )
	return;

    ci->listeners->channel_rcv_data(channel_name, sender_name, type, data);
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
				 user_channel_name(channel_name),
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
				 user_channel_name(channel_name),
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

    user_channel_name = user_channel_name(channel_name);
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
				 user_channel_name(channel_name),
				 message)));
}

/*
** create()
**
** duh.
*/
void create()
{
    class listener_pair pair;

    set_privilege(1);

    info = ([ ]);

    map_array(bodies(), (: register_body :));

    restore_object(SAVE_FILE, 1);
    if ( saved_listeners )
    {
	foreach ( pair in saved_listeners )
	{
	    object ob = find_object(pair->filename);

	    if ( ob )
		register_one(0, ob, pair->channel_name);
	}

	saved_listeners = 0;
    }
    if ( saved_hooks )
    {
	foreach ( pair in saved_hooks )
	{
	    object ob = find_object(pair->filename);

	    if ( ob )
		register_one(1, ob, pair->channel_name);
	}

	saved_hooks = 0;
    }
}

/*
** remove()
**
** Write out all listeners and hooks that are blueprints.  We'll reset
** them at creation time.
*/
void remove()
{
    string channel_name;
    class channel_info ci;

    saved_listeners = ({ });
    saved_hooks = ({ });

    foreach ( channel_name, ci in info )
    {
	object ob;

	foreach ( ob in ci->listeners - ({ 0 }) )
	{
	    string fname = file_name(ob);

	    if ( member_array('#', fname) == -1 )
	    {
		class listener_pair pair = new(class listener_pair);

		pair->channel_name = channel_name;
		pair->filename = fname;
		saved_listeners += ({ pair });
	    }
	}

	foreach ( ob in ci->hooked - ({ 0 }) )
	{
	    string fname = file_name(ob);

	    if ( member_array('#', fname) == -1 )
	    {
		class listener_pair pair = new(class listener_pair);

		pair->channel_name = channel_name;
		pair->filename = fname;
		saved_hooks += ({ pair });
	    }
	}
    }

    unguarded(1, (: save_object, SAVE_FILE :));
}


/*
** query_channels()
**
** Return the list of active channels
*/
nomask string * query_channels()
{
    return keys(info);
}

/*
** query_listeners()
**
** Return the listeners of a particular channel
*/
nomask object * query_listeners(string channel_name)
{
    if ( check_previous_privilege(1) )
    {
	class channel_info ci = info[channel_name];

	if ( ci )
	    return ci->listeners;
    }

    return 0;
}
