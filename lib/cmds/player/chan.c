/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** chan.c
**
** General channel command.
**
** 06-Nov-94. Created. Deathblade.
*/

//:PLAYERCOMMAND
//$$ see: channels, gossip, newbie
//The chan command is the central command for all channel
//communication.  Below are the various forms the command takes:
//
//chan			- list channels being listened to
//chan gossip		- find out whether the 'gossip' channel is
//			  begin listened to
//chan gossip /on		- turn listening on for the 'gossip' channel
//chan gossip /off	- stop listening to 'gossip'
//chan gossip /list	- find out who is listening to 'gossip'
//chan gossip /who	- same as /list
//chan gossip /last	- show the last 20 messages on the channel
//chan gossip <msg>	- send <msg> to all listeners
//chan gossip ;<feeling>	- perform <feeling> over the channel
//			  (e.g. chan gossip ;grin)
//chan gossip :<emote>	- send <emote> to all listeners
//			  (e.g. chan gossip :wants a big sword!)
//chan mine /new		- create the "mine" channel
//
//Of course, the 'gossip' may be replaced with any channel name you
//might want to use.
//
//chan will create a channel, if necessary, when you first start
//to listen to it.  There are (currently) no restrictions on the
//number or channels you may listen to or create.  Once you listen
//to a channel, you can simply use its name for operating the channel.
//
//Two player channels are standard: gossip and newbie
//
//Some examples:
//
//> chan
//You are listening to: gossip, newbie, mychannel
//> chan gossip /off
//You are no longer listening to 'gossip'.
//> mychannel /list
//Users listening to 'mychannel': Johndoe, Smurfhead, Tarzan
//> mychannel hello!
//[mychannel] Smurfhead: hello!
//> mychannel :turns a bright shade of blue
//[mychannel] Smurfhead turns a bright shade of blue.
//> mychannel ;laugh tarzan
//[mychannel] You laugh at Tarzan.
//>
//> [mychannel] Tarzan bops you on the nose.

inherit CMD;


void create()
{
  ::create();
  no_redirection();
}

private void main(string arg)
{
    string channel_name;
    int chan_type;

    if ( !arg || arg == "" || (wizardp(this_user()) && arg == "-d") )
    {
	string * channel_list;

	channel_list = this_user()->query_channel_list();
	if ( sizeof(channel_list) == 0 )
	    out("You are not listening to any channels.\n");
	else
	{
	    if ( arg != "-d" )
		channel_list = map(channel_list,
				   (: CHANNEL_D->user_channel_name($1) :));

	    out("You are listening to: " +
		  implode(channel_list, ", ") + ".\n");
	}
				     
	return;
    }

    if ( sscanf(arg, "%s %s", channel_name, arg) != 2 )
    {
	channel_name = arg;
	arg = "";
    }

    /* COMPATIBILITY: if there is no '_' in the name, prepend plyr_ */
//    if ( member_array('_', channel_name) == -1 )
//	channel_name = "plyr_" + channel_name;

    chan_type = channel_name[0..4] == "imud_";

    CHANNEL_D->cmd_channel(channel_name, arg, chan_type);
}
