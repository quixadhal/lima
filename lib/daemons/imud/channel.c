/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** channel.c -- intermud channel processing
**
** 95-May-15.  Deathblade.  Created.
** July 5, 1997:  Midhir@Mutants and Machinery added some 
**                channel listening/ignoring capabilities.
*/

#include <daemons.h>

nosave private int	filter_msg;

private int		chanlist_id;
private mapping		chanlist = ([ ]);
string*			listened_channels = ({});

void send_to_all(string type, mixed * message);
void send_to_user(string type, string mudname, string username,
		  mixed * message);
void send_to_router(string type, mixed * message);
void send_to_mud(string type, string mudname, mixed * message);
void return_error(string mudname, string username,
		  string errcode, string errmsg);
void add_cache_entry(string mudname, string username,
		     string visname, int gender);
string get_visname( string mudname, string username );


protected nomask int query_chanlist_id()
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
	if ( !strlen( data ) ) break;
	data = sprintf("$N %s", data);
	send_to_all("channel-e", ({ channel_name, sender_name, data }));
	break;

    case "soul":
//### need to work on using channel-t appropriately
	if ( sizeof( data[0] ) != 2 )
	{
	    data = data[<1][<1];
	    if ( data[<1] == '\n' )
	      data = data[0..<2];
	    send_to_all("channel-e",
			({ channel_name, sender_name, data }));
	} else
	{
	    string target_mud = data[0][1]->query_mud_name() || mud_name();
	    string target_name = data[0][1]->query_userid();
	    string all_mess = data[<1][<1];
	    string targ_mess = data[<1][1];

	    DBBUG(data);
	    if ( all_mess[<1] == '\n' ) all_mess = all_mess[0..<2];
	    if ( targ_mess[<1] == '\n' ) targ_mess = targ_mess[0..<2];
	    
	    send_to_all("channel-t",
			({ channel_name,
			   target_mud,  // Target mud
			   target_name, // Target name
			   all_mess, // Message to all
			   targ_mess,  // Target message
			   data[0][0]->query_name(), // Sender name
			   get_visname( target_mud, target_name ) ||
			     capitalize( target_name ) }) );

	}
	break;
    }
}

protected nomask void rcv_chanlist_reply(string orig_mud, string orig_user,
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

protected nomask void rcv_chan_who_req(string orig_mud, string orig_user,
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

protected nomask void rcv_chan_who_reply(string orig_mud, string orig_user,
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
nosave private string * april_phrases = ({
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

protected nomask void rcv_channel_m(string orig_mud, string orig_user,
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


protected nomask void rcv_channel_e(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
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

protected nomask void rcv_channel_t(string orig_mud, string orig_user,
				 string targ_user, mixed * message)
{
    
    string orig_name,targ_name;
    string targ_mess, all_mess;

    if ( mud_name() == orig_mud)
      return ;

    orig_name = message[5] + "@" + orig_mud;
	
    all_mess = replace_string( message[3]+"\n","$N", orig_name );

    if ( mud_name() == message[1] )
    {
	object targ_ob = find_body( lower_case( message[2] ) );

	targ_name = message[6];

	if ( targ_ob )
	{
            targ_mess = replace_string( message[4]+"\n","$N", orig_name );

            all_mess = replace_string( all_mess, "$O", targ_name );

	    filter_msg = 1;
    
	    CHANNEL_D->deliver_soul( "imud_"+message[0],
				    ({ ({ targ_ob }),
				       ({ targ_mess, all_mess }) }) );
	    return ;
	}
    } else
      targ_name = message[6] + "@" + message[1];

    all_mess = replace_string( all_mess, "$O", targ_name );
    
    CHANNEL_D->deliver_channel( "imud_"+message[0], all_mess );
}

protected nomask void rcv_chan_user_req(string orig_mud, string orig_user,
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
	gender = (gender + 2) % 3;	/* Lima -> I3 */
	
	send_to_mud("chan-user-reply", orig_mud,
		    ({ message[0], p->query_name(), gender }));
    }
}

protected nomask void rcv_chan_user_reply(string orig_mud, string orig_user,
				       string target_user, mixed * message)
{
    int gender = (message[2] + 1) % 3;	/* I3 -> Lima */

    add_cache_entry(orig_mud, message[0], message[1], gender);

    /* ### do rest of emote now... */
}

protected nomask void chan_startup()
{
    CHANNEL_D->register_channels(map_array(keys(chanlist),
					    (: "imud_" + $1 :)));
}
protected nomask void chan_shutdown()
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

nomask void listen_to_channel(string channel_name) {
    if (member_array(channel_name, listened_channels) == -1) 
       listened_channels += ({ channel_name });

    send_to_router("channel-listen", ({ channel_name, 1 }));
}

nomask void ignore_channel(string channel_name) {
    if (member_array(channel_name, listened_channels) != -1) 
       listened_channels -= ({ channel_name });
    send_to_router("channel-listen", ({ channel_name, 0 }));
}

nomask void ban_mud(string chan, string mud) {
    send_to_router("channel-admin", ({ chan, ({}), ({ mud })}));
}

nomask void relisten_all_channels() {
  foreach (string str in listened_channels) listen_to_channel(str);
  return;
}

nomask void listen_to_all_channels() {
  foreach (string str in keys(this_object()->query_chanlist())) {
          listen_to_channel(str);
          listened_channels += ({ str });
          continue;
  }
}

nomask string *query_listened_channels() { return copy(listened_channels); }
