/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";
inherit CLASS_BANISH_DATA;

nomask string module_name() {
    return "banish";
}

nomask string module_key() {
    return "b";
}

nomask string module_user() {
    return "[Mudlib:daemons]";
}

nomask string module_priv() {
    return "Mudlib:daemons";
}

private nomask void receive_banish_name(string name, string reason)
{
    if ( reason == "" )
    {
	write("Aborted.  The reason is required.\n");
    }
    else
    {
	BANISH_D->banish_name(name, reason);
	write("Done.\n");
    }
}


private nomask void receive_banish_site(string name, string reason)
{
    if ( reason == "" )
    {
	write("Aborted.  The reason is required.\n");
    }
    else
    {
	BANISH_D->banish_site(name, reason);
	write("Done.\n");
    }
}

private nomask void show_banishes(string header, class banish_data *list)
{
    string result = header + "\n" + repeat_string("-", 73) + "\n";

    foreach ( class banish_data b in list )
    {
	result += sprintf("%-20s : %s\n", b->item, b->reason);
    }
    more(result);
}

nomask void show_registered_users()
{
  string array users=BANISH_D->get_registered();
  string array result;
  if(!sizeof(users))
    {
      write("No users registered.\n");
      return;
    }
  result=({"Users Registered to login from banished sites  ---------------"});
  result+=users;
  more(result);
  return;
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "L",
	    action : (: show_banishes("Banished names", BANISH_D->show_banishes()[0]) :),
	    desc : "list banished names"),
	new(class command_info,
	    key : "l",
	    action : (: show_banishes("Banished sites", BANISH_D->show_banishes()[1]) :),
	    desc : "list banished sites"),
	new(class command_info,
	    key : "B",
	    args : ({ "Banish who? ", "Banish them why? " }),
	    proto : "[name] [reason]",
	    action : (: call_other, BANISH_D, "banish_name" :),
	    desc : "banish a name"),
	new(class command_info,
	    key : "b",
	    args : ({ "Banish which site? ", "Banish them why? " }),
	    proto : "[name] [reason]",
	    action : (: call_other, BANISH_D, "banish_site" :),
	    desc : "banish a site"),
	new(class command_info,
	    key : "U",
	    proto : "[name]",
	    args : ({ "Unbanish which name? " }),
	    action : (: call_other, BANISH_D, "unbanish_name" :),
	    desc : "unbanish a name"),
	new(class command_info,
	    key : "u",
	    proto : "[name]",
	    args : ({ "Unbanish which site? " }),
	    action : (: call_other, BANISH_D, "unbanish_site" :),
	    desc : "unbanish a site"),
	new(class command_info,
	    key : "R",
	    proto : "[who]",
	    args : ({ "Who? " }),
	    action : (: call_other,BANISH_D,"add_registered" :),
	    desc : "add a registered user" ) ,
	new(class command_info,
	    key : "r",
	    proto : "[name]",
	    args : ({ "Who? ", }),
    action : (: call_other,BANISH_D, "remove_registered" :),
	    desc : "remove a registered user" ),
	new(class command_info,
	    key : "s",
	    action : (: show_registered_users :),
	    desc : "show the registered users of a site"),
    });
}
