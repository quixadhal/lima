/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

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

private nomask void rcv_remove_channel(string channel_name)
{
    mapping chanlist = IMUD_D->query_chanlist();

    channel_name = lower_case(channel_name);
    if ( undefinedp(chanlist[channel_name]) )
    {
	write("** That channel does not exist.\n");
	return;
    }

    IMUD_D->remove_channel(channel_name);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list owned channels",
	    action : (: list_channels :)),
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
	    action : (: rcv_remove_channel :))
    });
}
