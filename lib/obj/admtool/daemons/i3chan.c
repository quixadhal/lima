/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";
inherit M_ANSI;

nomask string module_name() {
    return "I3 channels";
}

nomask string module_key() {
    return "i";
}

nomask string module_user() {
    return "[Mudlib:daemons]";
}

nomask string module_priv() {
    return "Mudlib:daemons";
}

private nomask void list_channels()
{
    mapping chanlist = IMUD_D->query_chanlist();

    chanlist = filter_mapping(chanlist,
			      (: $(chanlist)[$1][0] == mud_name() :));

    write("Owned Channels:");
    if ( sizeof(chanlist) == 0 )
    {
	write("    <none>\n");
	return;
    }

    write(implode(({ "" }) + keys(chanlist),
		  (: $1 + sprintf("    %s/%s\n", $2,
				  $(({"unrestricted",
					  "restricted",
					  "filtered"}))[$(chanlist)[$2][1]])
		   :)));
}

private nomask void rcv_add_channel(string channel_name)
{
    channel_name = lower_case(trim_spaces(channel_name));
    if ( channel_name == "" )
	return;

    if ( IMUD_D->query_chanlist()[channel_name] )
    {
	write("** That channel already exists.\n");
	return;
    }

//### need to get a channel type...
    IMUD_D->add_channel(channel_name);
}

private nomask void rcv_remove_channel(string channel_name) {
    mapping chanlist = IMUD_D->query_chanlist();

    channel_name = lower_case(channel_name);
    if ( undefinedp(chanlist[channel_name]) )
    {
	write("** That channel does not exist.\n");
	return;
    }

    IMUD_D->remove_channel(channel_name);
}

private nomask void rcv_listen_channel(string channel_name) {
    mapping chanlist = IMUD_D->query_chanlist();

    channel_name = lower_case(channel_name);
    if (undefinedp(chanlist[channel_name])) {
       write("** That channel does not exist.\n");
       return;
    }
    IMUD_D->listen_to_channel(channel_name);
    write("Receiving channel: "+channel_name+"\n");
}

private nomask void rcv_ignore_channel(string channel_name) {
    mapping chanlist = IMUD_D->query_chanlist();

    channel_name = lower_case(channel_name);
    if (undefinedp(chanlist[channel_name])) {
       write("** That channel does not exist.\n");
       return;
    }
    IMUD_D->ignore_channel(channel_name);
    write("Ignoring channel: "+channel_name+"\n");
}

private nomask void list_all_channels() {
    mapping chanlist = IMUD_D->query_chanlist();

    write(colour_table(keys(chanlist), this_user()->query_screen_width(), 4)+"\n");
}

private nomask void listened_channels() {
    string *chanlist = IMUD_D->query_listened_channels();
    write(colour_table(chanlist, this_user()->query_screen_width(), 4)+"\n");
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "O",
	    desc : "list owned channels",
	    action : (: list_channels :)),
        new(class command_info,
            key : "l",
            desc : "list channels which are being listened to",
            action : (: listened_channels :)),
        new(class command_info,
            key : "C",
            desc : "list all channels",
            action : (: list_all_channels :)),
	new(class command_info,
	    key : "a",
	    proto : "[name]",
	    desc : "add owned channel",
	    args : ({ "New channel name? " }),
	    action : (: rcv_add_channel :)),
	new(class command_info,
	    key : "r",
	    proto : "[name]",
	    desc : "remove owned channel",
	    args : ({ "Channel name to remove? " }),
	    action : (: rcv_remove_channel :)),
        new(class command_info,
            key: "L",
            proto : "[name]",
            desc  : "listen to an intermud channel",
            args  : ({ "Channel name to listen to? " }),
            action : (: rcv_listen_channel :)),
        new(class command_info,
            key: "I",
            proto : "[name]",
            desc  : "ignore an intermud channel",
            args  : ({ "Channel name to ignore? " }),
            action : (: rcv_ignore_channel :)),
    });
}
