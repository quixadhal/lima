/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** channel.c -- intermud channel processing
**
** 95-May-15.  Deathblade.  Created.
*/

#include <daemons.h>

static private int	filter_msg;

private int		chanlist_id;
private mapping		chanlist = ([ ]);

void send_to_all(string type, mixed * message);
void send_to_user(string type, string mudname, string username,
		  mixed * message);
void send_to_router(string type, mixed * message);
void send_to_mud(string type, string mudname, mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
void add_cache_entry(string mudname, string username,
		     string visname, int gender);


static nomask int query_chanlist_id()
{
    return chanlist_id;
}

nomask void channel_rcv_data(string channel_name,
			     string sender_name,
			     string type,
			     mixed data)
{
    if ( filter_msg )
    {
	filter_msg = 0;
	return;
    }

    channel_name = channel_name[5..];	// skip "imud_"

    switch ( type )
    {
    case "tell":
	send_to_all("channel-m", ({ channel_name, sender_name, data }));
	break;

    case "emote":
	data = sprintf("$N %s", data);
	send_to_all("channel-e", ({ channel_name, sender_name, data }));
	break;

    case "soul":
//### need to work on using channel-t appropriately
	data = data[1][<1];
	if ( data[<1] == '\n' )
	    data = data[0..<2];
	send_to_all("channel-e",
		    ({ channel_name, sender_name, data }));
	break;
    }
}

static nomask void rcv_chanlist_reply(string orig_mud, string orig_user,
				      string targ_user, mixed * message)
{
    string channel_name;
    mixed channel_data;
    
    chanlist_id = message[0];

    foreach ( channel_name, channel_data in message[1] )
    {
	string int_name = "imud_" + channel_name;
	    
	if ( !channel_data )
	{
	    map_delete(chanlist, channel_name);
	    CHANNEL_D->unregister_channels(({ int_name }));
	}
	else
	{
	    chanlist[channel_name] = channel_data;
	    CHANNEL_D->register_channels(({ int_name }));
	}
    }
}

static nomask void rcv_chan_who_req(string orig_mud, string orig_user,
				    string targ_user, mixed * message)
{
    object * listeners = CHANNEL_D->query_listeners(message[0]);

    if ( !listeners )
    {
	return_error(orig_mud, orig_user, "unk-channel",
		     sprintf("Never heard of '%s'", message[0]));
    }
    else
    {
	listeners -= ({ 0, this_object() });
	send_to_user("chan-who-reply", orig_mud, orig_user,
		     ({ message[0], listeners->query_name() }));
    }
}

static nomask void rcv_chan_who_reply(string orig_mud, string orig_user,
				      string targ_user, mixed * message)
{
    object p;

    p = find_user(targ_user);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("'%s' is unknown", targ_user));
    }
    else
    {
	tell(p, sprintf("%s@%s listeners: ",
				    message[0], orig_mud) +
			    implode(message[1], ", ") + ".\n");
    }
}

/* ### to support the April Fool's stuff below... */
static private string * april_phrases = ({
    // "yes, you are!",
    "not you... Ohara is the fool!",
    "but Zakk is a bigger fool!",
    "well, duh! we knew that!",
    "yes... anyone who logs onto IdeaExchange is a fool!",
    "of course! anyone who idles on a hostile mud is a fool.",
    "well, Beek's the bigger fool... he admitted to singing Live's \"I Alone\" for a piece of tail.",
    "so was Rust's mom ... and look at the result!",
    "yeah, but at least you didn't choose a mud name as lame as 'Deathblade'.",
    "nog, that's what your mom told me last night.",
    "then move to StarMUD; you'll fit in better",
    "I think your mom said that last night, but she had her mouth full at the time.",
    "we hear you like small children, too.",
    "well, at least you're not as bad as those who use Heaven 7.",
    "well, at least you're not as bad as those who use Heaven 7.",
    "really? You must use TMI.",
    "you're so lame, that's your *best* quality.",
    "you're a Discworld admin, right?",
    "Oh god ... it's _another_ newbie who wants to write their own lib.",
    "And to add salt to your wounds:  'She' was a 'He'.",
});
string query_userid()
{
    return "limabean";
}

static nomask void rcv_channel_m(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
    if ( orig_mud == mud_name() )
	return;

    filter_msg = 1;
    CHANNEL_D->deliver_tell("imud_" + message[0],
			     sprintf("%s@%s", message[1], orig_mud),
			     message[2]);

#if 0
    /* ### April Fool's ... respond to IdeaExchange :-) */
    if ( message[2] == "I am a fool!" )
    {
	object tu = this_user();
	set_this_player(this_object());
	CHANNEL_D->deliver_tell("imud_" + message[0],
				"Lima Bean",
				choice(april_phrases));
	set_this_player(tu);
    }
#endif
}


static nomask void rcv_channel_e(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
    object p;

    if ( orig_mud == mud_name() )
	return;

    if ( sizeof(message) != 3 )
    {
	return_error(orig_mud, orig_user, "bad-pkt",
		     "Bad channel-e format. Report to your admin.");
	return;
    }

    /*
    ** NOTE: we don't need to set filter_msg here.  The deliver_channel()
    ** message will not pass these back to us.  Only the tell/emote/soul
    ** will pass the data through to us.
    */
    CHANNEL_D->deliver_channel("imud_" + message[0],
				replace_string(message[2], "$N",
					       sprintf("%s@%s",
						       message[1],
						       orig_mud)));
}

static nomask void rcv_channel_t(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
#ifdef OLD_BOGUS_CODE
    object p;

    if ( orig_mud == mud_name() )
	return;

    filter_msg = 1;
    if ( message[1] == mud_name() &&
	p = find_body(message[2]) )
    {
	mixed * soul;

	soul = ({ ({ 0, p }), ({ 0, message[4], message[3] }) });
	CHANNEL_D->deliver_soul("imud_" + message[0], soul);
    }
    else
    {
	CHANNEL_D->deliver_channel("imud_" + message[0], message[3]);
    }
#endif
}

static nomask void rcv_chan_user_req(string orig_mud, string orig_user,
				     string target_user, mixed * message)
{
    object p;

    p = find_body(message[0]);
    if ( !p )
    {
	return_error(orig_mud, orig_user, "unk-user",
		     sprintf("'%s' is unknown", message[0]));
    }
    else
    {
	int gender = p->query_gender();

	/* map to I3's concept of gender: male(0), female(1), neuter(2)
	   ours is: neuter(0), male(1), female(2) */
	gender = (gender + 1) % 3;	/* Lima -> I3 */
	
	send_to_mud("chan-user-reply", orig_mud,
		    ({ message[0], p->query_name(), gender }));
    }
}

static nomask void rcv_chan_user_reply(string orig_mud, string orig_user,
				       string target_user, mixed * message)
{
    int gender = (message[2] + 2) % 3;	/* I3 -> Lima */

    add_cache_entry(orig_mud, message[0], message[1], gender);

    /* ### do rest of emote now... */
}

static nomask void chan_startup()
{
    CHANNEL_D->register_channels(map_array(keys(chanlist),
					    (: "imud_" + $1 :)));
}
static nomask void chan_shutdown()
{
    CHANNEL_D->unregister_channels();
}

nomask mapping query_chanlist()
{
    return copy(chanlist);
}

nomask void add_channel(string channel_name, int channel_type)
{
    send_to_router("channel-add", ({ channel_name, channel_type }));
}

nomask void remove_channel(string channel_name)
{
    send_to_router("channel-remove", ({ channel_name }));
}

nomask void remote_listeners(string mudname, string channel_name)
{
    send_to_mud("chan-who-req", mudname, ({ channel_name }));
}
