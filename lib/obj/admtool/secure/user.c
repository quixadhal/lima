/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

#define SECS_PER_DAY	(24 * 60 * 60)

nomask string module_name() {
    return "user";
}

nomask string module_key() {
    return "u";
}

nomask string module_user() {
    return "[admin]";
}

nomask mixed module_priv() {
    return 1;
}

private nomask void nuke_user(string userid, int skip_save)
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
}

private nomask void confirm_nuking(string name, string str)
{
    str = lower_case(str);
    if ( str != "y" && str != "yes" )
    {
	write("Nuke aborted!\n");
	return;
    }

    nuke_user(name, 0);
}

private nomask void receive_name_for_nuking(string name)
{
    name = lower_case(name);
    if( !user_exists( name ))
    {
	printf( "%s doesn't exist", capitalize(name));
	return;
    }

    printf("Are you sure you want to nuke '%s' ? ", capitalize(name));
    modal_simple((: confirm_nuking, name :));
}

private nomask void receive_name_for_wiz(string name)
{
    object ob;
    string err;

    name = lower_case(name);
    if ( SECURE_D->query_is_wizard(name) )
    {
	printf("** '%s' is already a wizard.\n", capitalize(name));
	if ( !is_directory(WIZ_DIR "/"+name) )
	{
	    printf("However, %s/%s doesn't exist.  Creating...\n",
	      WIZ_DIR, name);
	    mkdir(WIZ_DIR "/" + name);
	    SECURE_D->set_protection(WIZ_DIR "/" + name, 1, name + ":");
	}
	return;
    }
    err = SECURE_D->create_wizard(name);
    if ( err )
    {
	printf("** Error: %s\n", err);
	return;
    }
    mkdir(WIZ_DIR "/" + name);
    SECURE_D->set_protection(WIZ_DIR "/" + name, 1, name + ":");

    printf("'%s' is now a wizard.\n", capitalize(name));

    //### switch to an action?
    ob = find_user(name);
    if ( ob )
    {
	tell(ob, "You are now a wizard.  Changing bodies...\n");

	ob->force_me("su");
    }
}

private nomask void receive_name_for_dewiz(string name)
{
    object ob;
    string err;

    name = lower_case(name);
    if ( adminp(name) )
    {
	printf("** '%s' is an admin and cannot be dewizzed.\n",
	  capitalize(name));
	return;
    }
    if ( !SECURE_D->query_is_wizard(name) )
    {
	printf("** '%s' is not a wizard.\n", capitalize(name));
	return;
    }
    err = SECURE_D->delete_wizard(name);
    if ( err )
    {
	printf("** Error: %s\n", err);
	return;
    }
    SECURE_D->set_protection(WIZ_DIR "/" + name, 1, -1);

    printf("'%s' is no longer a wizard.\n", capitalize(name));

    //### switch to an action?
    ob = find_user(name);
    if ( ob )
    {
	tell(ob, "You have lost your wizard status.\n");

	ob->force_me("su");
    }
}

#ifdef USE_WIZ_POSITION
private nomask void receive_position_for_wiz(string name, string position)
{
    USER_D->set_variable(lower_case(name), "wiz_position", position);

    printf("%s's position has been set to: %s\n",
      capitalize(lower_case(name)), position);
}
#endif

private nomask void confirm_purge(mixed * times, string str)
{
    str = lower_case(str);
    if ( str != "y" && str != "yes" )
    {
	write("Purge aborted!\n");
	return;
    }

    foreach ( mixed * info in times )
    nuke_user(info[1], 1);

    LAST_LOGIN_D->save_me();
}

private nomask void receive_days_for_purge(string days)
{
    int limit = time() - (to_int(days) * SECS_PER_DAY);
    mixed * times = filter(LAST_LOGIN_D->query_times(),
      (: $1[0] <= $(limit) :));

    printf("You will nuke %d users that have not logged on after %s.\n"
      "Are you sure? ",
      sizeof(times), ctime(limit));
    modal_simple((: confirm_purge, times :));
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "n",
	    proto : "[name]",
	    args : ({ "Who should be nuked? " }),
	    action : (: receive_name_for_nuking :),
	    desc : "nuke a user"),
	new(class command_info,
	    key : "w",
	    proto : "[name]",
	    args : ({ "Who should be wizzed? " }),
	    action : (: receive_name_for_wiz :),
	    desc : "wiz a user"),
	new(class command_info,
	    key : "d",
	    proto : "[name]",
	    args : ({ "Who should be de-wizzed? " }),
	    action : (: receive_name_for_dewiz :),
	    desc : "dewiz a suer"),
#ifdef USE_WIZ_POSITION
	new(class command_info,
	    key : "p",
	    proto : "[name] [position]",
	    args : ({ "Set position for who? ", "Set %s's postion to what? " }),
	    action : (: receive_position_for_wiz :),
	    desc : "give a wizard a position"),
#endif
	new(class command_info),
	new(class command_info,
	    key : "P",
	    proto : "[days]",
	    args : ({ "How many days since their last login? " }),
	    action : (: receive_days_for_purge :),
	    desc : "Purge users not logged in within N days"),
    });
}
