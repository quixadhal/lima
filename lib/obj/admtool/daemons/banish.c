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

private nomask void pave_user(string userid, int skip_save)
{
    object o;
    mixed err;

    if ( o = find_user(userid) )
    {
	o->receive_private_msg("Sorry.  You're being nuked.\n");
	o->quit();
    }

    MAILBOX_D->get_mailbox(userid)->nuke_mailbox(1);
    MAILBOX_D->unload_mailbox(userid);

    /* remove a bunch of files. note: some might not exist. */
    err = rm(LINK_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(USER_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(PSHELL_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(WSHELL_PATH(userid) + __SAVE_EXTENSION__);

    LAST_LOGIN_D->remove_user(userid, skip_save);

    err = SECURE_D->delete_wizard(userid);

//### deal with clearing privs and stuff
//### this should be enough, but may need more thought (this was a quicky)
//### need to set it to something like @disabled so that unguarded() code
//### in the wiz dir doesn't have priv 1 now.
    SECURE_D->set_protection(WIZ_DIR "/" + userid, 1, -1);
    
    printf("'%s' has been nuked.\n", capitalize(userid));

    BANISH_D->banish_name(userid, "Nuked and paved over.");
}


private nomask void confirm_paving(string name, string str)
{
    str = lower_case(str);
    if ( str != "y" && str != "yes" )
    {
	write("Nuke aborted!\n");
	return;
    }

    pave_user(name, 0);
}

private nomask void receive_name_for_paving(string name)
{
    name = lower_case(name);
    modal_simple((: confirm_paving, name :),
		 sprintf("Are you sure you want to nuke '%s' ? ",
			 capitalize(name)));
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
	    desc : "banish a name"),
	new(class command_info,
	    key : "b",
	    args : ({ "Banish which site? ", "Banish them why? " }),
	    proto : "[name] [reason]",
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
	    key : "n",
	    proto : "[name]",
	    priv : 1,
	    args : ({ "Who should be nuked and paved? " }),
	    desc : "Nuke and Pave a user",
	    who : "[admin]")
    });
}
