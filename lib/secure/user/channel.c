/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * Handling of channels for the user
 * 
 * Created 04-17-2000 by Tigran
 */

#define USER_CHANNELS ({"newbie", "gossip" })
#define WIZ_CHANNELS ({"wiz", "errors", "announce", "news" })

void save_me();
varargs void receive_private_msg(string msg,object array exclude, 
				 int message_type,mixed other);

private string array channel_list;

/* 
 * Set up channels for the new player and/or wiz
 */
protected void setup_channels() {
  channel_list=USER_CHANNELS;
  if(wizardp(this_object()))
    channel_list+=WIZ_CHANNELS;
  foreach(string chan in channel_list)
    printf("Tuning in the %s channel (%s /on)\n",chan,chan);
}

/* 
 * Initialize the users channels
 */
protected void initialize_channels() {
  if(undefinedp(channel_list))
    setup_channels();
  CHANNEL_D->register_channels(channel_list);
}

/* 
 * Ok, we're leaving.  Unregister us from the channels
 */
protected void exit_all_channels() {
  CHANNEL_D->unregister_channels(channel_list);
}

//:FUNCTION add_channel
//Add a channel that the user is listening to
void add_channel(string which_channel) {
    channel_list += ({ which_channel });
    CHANNEL_D->register_channels(({ which_channel }));
    save_me();
}

//:FUNCTION remove_channel
//Remove a channel that the user is listening to.
void remove_channel(string which_channel) {
    channel_list -= ({ which_channel });
    CHANNEL_D->unregister_channels(({ which_channel }));
    save_me();
}

//:FUNCTION query_channel_list
//List all of the channels that the user is listening to.
string array query_channel_list() {
    return channel_list;
}

void channel_rcv_string(string channel_name, string msg)
{
  receive_private_msg(msg);
}

void channel_rcv_soul(string channel_name, array data)
{
    string msg;

    if ( data[0][0] == this_object() )
	msg = data[1][0];
    else if ( sizeof(data[0]) == 2 && data[0][1] != this_object() )
	msg = data[1][2];
    else
	msg = data[1][1];

    receive_private_msg(msg);
}

void channel_add(string which_channel)
{
    channel_list += ({ which_channel });
    CHANNEL_D->register_channels(({ which_channel }));
}

void channel_remove(string which_channel)
{
    channel_list -= ({ which_channel });
    CHANNEL_D->unregister_channels(({ which_channel }));
}
