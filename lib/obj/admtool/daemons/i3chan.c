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

private nomask void detail_list_channels()
{
  mapping chanlist = IMUD_D->query_chanlist();

  write("All Channels:\n");
  if ( sizeof(chanlist) == 0 )
  {
    write("    <none>\n");
    return;
  }

  write(implode(({ "" }) + sort_array(keys(chanlist),1),
      (: $1 + sprintf("    %-20s %-20s %s\n", $2,
      $(chanlist)[$2][0],
      $(({"unrestricted", "restricted", "filtered"}))[$(chanlist)[$2][1]],
      )
      :)));
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

    write(implode(({ "" }) + sort_array(keys(chanlist),1),
		  (: $1 + sprintf("    %s/%s\n", $2,
				  $(({"unrestricted",
					  "restricted",
					  "filtered"}))[$(chanlist)[$2][1]])
		   :)));
}

private nomask void rcv_add_channel(string channel_name, int type)
{
  channel_name = lower_case(trim_spaces(channel_name));
  if ( channel_name == "" )
    return;

  if ( IMUD_D->query_chanlist()[channel_name] )
  {
    write("** That channel already exists.\n");
    return;
  }

  IMUD_D->add_channel(channel_name, to_int(type));
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

private nomask void rcv_listen_channel(string channel_name)
{
  mapping chanlist = IMUD_D->query_chanlist();

  channel_name = lower_case(channel_name);
  if (undefinedp(chanlist[channel_name]))
  {
     write("** That channel does not exist.\n");
     return;
  }
  IMUD_D->listen_to_channel(channel_name);
  write("Receiving channel: "+channel_name+"\n");
}

private nomask void rcv_ignore_channel(string channel_name)
{
  mapping chanlist = IMUD_D->query_chanlist();

  channel_name = lower_case(channel_name);
  if (undefinedp(chanlist[channel_name]))
  {
     write("** That channel does not exist.\n");
     return;
  }
  IMUD_D->ignore_channel(channel_name);
  write("Ignoring channel: "+channel_name+"\n");
}

private nomask void list_all_channels()
{
  mapping chanlist = IMUD_D->query_chanlist();

  write(colour_table(sort_array(keys(chanlist),1), this_user()->query_screen_width(), 4)+"\n");
}

private nomask void listened_channels()
{
  string *chanlist = sort_array(IMUD_D->query_listened_channels(),1);
  write(colour_table(chanlist, this_user()->query_screen_width(), 4)+"\n");
}

private nomask void allow_mud_on_our_channel(string channel_name, string allowed_mud_name)
{
  mapping chanlist = IMUD_D->query_chanlist();

  channel_name = lower_case(channel_name);
  if (undefinedp(chanlist[channel_name]))
  {
    write("** That channel does not exist.\n");
    return;
  }
  IMUD_D->allow_mud_on_our_channel(channel_name, allowed_mud_name);
}

private nomask void disallow_mud_on_our_channel(string channel_name, string  disallowed_mud_name)
{
  mapping chanlist = IMUD_D->query_chanlist();

  channel_name = lower_case(channel_name);
  if (undefinedp(chanlist[channel_name]))
  {
    write("** That channel does not exist.\n");
    return;
  }
  IMUD_D->disallow_mud_on_our_channel(channel_name, disallowed_mud_name);
}

private nomask void rcv_block_mud(string mudname)
{
  string array muds = IMUD_D->query_barred();

  mudname = IMUD_D->canon_mudname(mudname);
  if ( !mudname )
  {
    write("** That mud does not appear to exist.\n");
    return;
  }

  IMUD_D->add_barred(mudname);
}

private nomask void rcv_unblock_mud(string mudname)
{
  string array muds = IMUD_D->query_barred();

  mudname = IMUD_D->canon_mudname(mudname);
  if ( !mudname )
  {
    write("** That mud does not appear to exist.\n");
    return;
  }
  if (member_array(mudname, IMUD_D->query_barred())<0 )
  {
    write("** That mud does not appear to be barred.\n");
    return;
  }

  IMUD_D->remove_barred(mudname);
}

private nomask void list_blocked_muds()
{
  string array muds = IMUD_D->query_barred();

  write(colour_table(sort_array(muds,1), this_user()->query_screen_width(), 4)+"\n");
}

nomask class command_info array module_commands()
{
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
      key : "D",
      desc : "Detailed list of channels",
      action : (: detail_list_channels :)),

    new(class command_info,
	    key : "a",
	    proto : "[name] [type 0-2]",
	    desc : "add owned channel",
	    args : ({ "New channel name? ", "Add type (0 = unrestricted, 1 = restricted, 2 = filtered)" }),
	    action : (: rcv_add_channel :)),

   new(class command_info,
	    key : "r",
	    proto : "[name]",
	    desc : "remove owned channel",
	    args : ({ "Channel name to remove? " }),
	    action : (: rcv_remove_channel :)),

    new(class command_info,
      key : "A",
      proto : "[name]",
      desc : "allow mud to our owned channel",
      args : ({ "Channel name? ", "Allowed mud? " }),
      priv : "Mudlib:daemons",
      action : (: allow_mud_on_our_channel :)),

    new(class command_info,
      key : "D",
      proto : "[name]",
      desc : "disallow mud from our owned channel",
      args : ({ "Channel name? ", "Disallowed mud name? " }),
      priv : "Mudlib:daemons",
      action : (: disallow_mud_on_our_channel :)),

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
      new(class command_info,

      key: "b",
      proto : "[name]",
      desc  : "block a mud from intermud queries",
      args  : ({ "Mud name to block? " }),
      priv : "Mudlib:daemons",
      action : (: rcv_block_mud :)),

      new(class command_info,
      key: "u",
      proto : "[name]",
      desc  : "unblock a mud from intermud queries",
      args  : ({ "Mud name to unblock? " }),
      priv : "Mudlib:daemons",
      action : (: rcv_unblock_mud :)),

      new(class command_info,
      key: "B",
      desc  : "list muds blocked from intermud queries",
      priv : "Mudlib:daemons",
      action : (: list_blocked_muds :)),

    });
}
