/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "news";
}

nomask string module_key() {
    return "n";
}

nomask string module_user() {
    return "[Mudlib:daemons]";
}

nomask string module_priv() {
    return "Mudlib:daemons";
}

private nomask void list_newsgroups()
{
    string* grouplist = NEWS_D->get_groups();

    if ( sizeof(grouplist) == 0 )
    {
	write("    <none>\n");
	return;
    }
    grouplist = sort_array(grouplist, 1);
    more(grouplist);

}

private nomask void rcv_add_newsgroup(string str)
{
    str = lower_case(trim_spaces(str));
    if ( str == "" )
	return;

    if ( member_array(str, NEWS_D->get_groups()) != -1 )
    {
	write("** That group already exists.\n");
	return;
    }
    NEWS_D->add_group(str);
    printf("** Group '%s' added.\n", str);
}

private nomask void rcv_remove_newsgroup(string group_name)
{
    string* grouplist = NEWS_D->get_groups();
    
    if ( !group_name )
    {
	write("** no group name supplied.\n");
	return;
    }

    group_name = lower_case(trim_spaces(group_name));
    if ( member_array(group_name, grouplist) == -1 )
    {
	write("** That newsgroup does not exist.\n");
	return;
    }

    NEWS_D->remove_group(group_name);
    printf("** Group '%s' removed.\n", group_name);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list newsgroups",
	    action : (: list_newsgroups :)),
	new(class command_info,
	    key : "a",
	    proto : "[name]",
	    desc : "add a newsgroup",
	    args : ({ "New newsgroup name? " }),
	    action : (: rcv_add_newsgroup :)),
	new(class command_info,
	    key : "r",
	    proto : "[name]",
	    desc : "remove a newsgroup",
	    args : ({ "Remove which newsgroup? " }),
	    action : (: rcv_remove_newsgroup :))
    });
}
