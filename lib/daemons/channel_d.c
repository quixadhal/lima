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
**	unregister_channels()	unregister a set of channels
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
#include <channel.h>

inherit M_ACCESS;
inherit M_GRAMMAR;

//inherit CLASS_CHANNEL_INFO;   picked up from channel/cmd

inherit __DIR__ "channel/cmd";
//inherit __DIR__ "channel/moderation";
#include "/daemons/channel/moderation.c"

/*
** Store some permanent listeners and the permanency list here
*/
#define SAVE_FILE	"/data/daemons/channel_d"

/*
** This channel information.  It specifies channel_name -> channel_info.
*/
private static mapping info;

/*
** This mapping contains which channels should not be auto-purged (keys)
** and their flags (values)
*/
private mapping permanent_channels = ([ ]);

/*
** The listener information is only saved on a "nice" shutdown (remove()).
** On a hard shutdown (mud crash), we don't care cuz the listeners and
** hooks will reattach when they load.  So you'll find changes to the
** permanent_channels will save, but these two variables will usually be
** zero at those times.
*/
class listener_pair
{
    string channel_name;
    string filename;
}
private class listener_pair *	saved_listeners;
private class listener_pair *	saved_hooks;


nomask void save_me()
{
    unguarded(1, (: save_object, SAVE_FILE :));
}

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

static nomask void create_channel(string channel_name)
{
    class channel_info	ci;

    if ( info[channel_name] )
	error("channel already exists\n");

    ci = new(class channel_info);
    ci->name		= extract_channel_name(channel_name);
    ci->listeners	= ({ });
    ci->hooked		= ({ });
    ci->history		= ({ });

    info[channel_name] = ci;
}

private void register_one(int add_hook, object listener, string channel_name)
{
    class channel_info	ci = info[channel_name];

    if ( !ci )
    {
	/*
	** Note: the registration interface allows for auto-creation.
	** The user commands request that /new be used, though.  This
	** allows a user to create a channel and listen to it, and it
	** will "remain" even across sessions.
	*/
	create_channel(channel_name);
	ci = info[channel_name];
    }

    /* enforce the channel restrictions now */
    /* ### not super secure, but screw it :-) */
    if ( (ci->flags & CHANNEL_WIZ_ONLY) && !wizardp(this_user()) )
    {
	/* ### don't error... might prevent somebody from logging in */
	return;
    }
    if ( (ci->flags & CHANNEL_ADMIN_ONLY) && !adminp(this_user()) )
    {
	/* ### don't error... might prevent somebody from logging in */
	return;
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

static nomask void test_for_purge(string channel_name)
{
    class channel_info ci = info[channel_name];

    if ( sizeof(ci->listeners) + sizeof(ci->hooked) == 0 )
	map_delete(info, channel_name);
}

private void unregister_one(string channel_name, object listener)
{
    class channel_info ci = info[channel_name];

    if ( ci )
    {
	ci->listeners -= ({ listener });

	/* purge the channel if it isn't permanent */
	if ( undefinedp(permanent_channels[channel_name]) )
	    test_for_purge(channel_name);
    }
}

private void register_body(object body)
{
    string * names;

    if ( !(names = body->query_channel_list()) )
	return;

    map_array(names, (: register_one, 0, body :));
}

static nomask void set_permanent(string channel_name, int is_perm)
{
    int no_exist = undefinedp(permanent_channels[channel_name]);

    if ( is_perm && no_exist )
    {
	class channel_info ci = info[channel_name];

	permanent_channels[channel_name] = ci->flags;
	save_me();
    }
    else if ( !is_perm && !no_exist )
    {
	map_delete(permanent_channels, channel_name);
	save_me();
    }
}

static nomask void set_flags(string channel_name, int flags)
{
    class channel_info ci = info[channel_name];

    ci->flags = flags;

    if ( !undefinedp(permanent_channels[channel_name]) )
    {
	permanent_channels[channel_name] = flags;
	save_me();
    }
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
** Un-register previous_object() from a specific set of channels.  If
** the list is 0, then unregister from all channels.
*/
nomask void unregister_channels(string * names)
{
    if ( !names )
	names = keys(info);

    map_array(names, (: unregister_one :), previous_object());
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

    ci->history += ({ str });
    if ( sizeof(ci->history) > CHANNEL_HISTORY_SIZE )
	ci->history[0..0] = ({ });

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
		   sprintf("[%s] %s\n",
			   user_channel_name(channel_name),
			   str));
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

    ci->history += ({ data[1][<1] });
    if ( sizeof(ci->history) > CHANNEL_HISTORY_SIZE )
	ci->history[0..0] = ({ });

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
		   sprintf("[%s] %s: %s\n",
			   user_channel_name(channel_name),
			   sender_name,
			   punctuate(message)));
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
    if( !sizeof( message ))
    {
    write( "Emote what?\n" );
        return;
}
    sender_name = find_sender_name(sender_name);

    deliver_data(channel_name, sender_name, "emote", message);
    deliver_string(channel_name,
		   sprintf("[%s] %s %s\n",
			   user_channel_name(channel_name),
			   sender_name,
			   punctuate(message)));
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
		     (: sprintf($("["+user_channel_name+"] %s"), $1) :))
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
		   sprintf("[%s] (%s)\n",
			   user_channel_name(channel_name),
			   message));
}

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

    foreach ( string channel_name, int flags in permanent_channels )
    {
	class channel_info ci;

	if ( !info[channel_name] )
	    create_channel(channel_name);

	ci = info[channel_name];
	ci->flags = flags;
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

    save_me();
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
    if ( check_previous_privilege(1) ||
	 previous_object() == find_object(IMUD_D) )
    {
	class channel_info ci = info[channel_name];

	if ( ci )
	    return ci->listeners;
    }

    return 0;
}

nomask mapping query_permanent()
{
    return copy(permanent_channels);
}

/*
** make_name_list()
**
** Make a list of names given an array of players.
*/
nomask string make_name_list(mixed * list)
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
** is_valid_channel()
**
** Is the given string a valid channel name (as in command name) ?  A list
** of internal names should be provided (such as the list a player is
** registered with).  The internal channel name will be returned, or 0
** if the name does not correspond to a channel.
**
** Note: permanent channels will always be valid.
*/
nomask string is_valid_channel(string which, string * list)
{
    foreach ( string name in list )
	if ( info[name] && ((class channel_info)info[name])->name == which )
	    return name;
    foreach ( string name in keys(permanent_channels) )
	if ( info[name] && ((class channel_info)info[name])->name == which )
	    return name;
    return 0;
}
