/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";


nomask string module_name() {
    return "(mail) group";
}

nomask string module_key() {
    return "g";
}

private nomask void list_groups()
{
    string * groups;

    groups = keys(GROUP_D->get_group_data());
    if ( !sizeof(groups) )
    {
	write("There are no groups.\n");
    }
    else
    {
	printf("The following groups exist:\n%-#78s\n",
	       implode(sort_array(groups, 1), "\n") + "\n");
    }
}

private nomask void show_group_members(string group)
{
    string * members;

    group = lower_case(group);
    members = GROUP_D->get_group_data()[group];
    if ( !members )
    {
	printf("There is no group named '%s'.\n", group);
    }
    else
    {
	printf("Group %s:\n", group);

	switch ( sizeof(members) )
	{
	case 0:
	    write("Members: (none)\n");
	    break;

	case 1:
	    printf("Member:  %s\n", members[0]);
	    break;

	default:
	    printf("Members: %s\n", implode(members, ", "));
	    break;
	}
    }
}

private nomask void create_group(string group)
{
    group = lower_case(group);

    GROUP_D->add_group(group);

    printf("Group %s created.\n", group);
}

private nomask void delete_group(string group)
{
    group = lower_case(group);

    GROUP_D->remove_group(group);

    printf("Group %s deleted.\n", group);
}

private nomask void show_users_groups(string user)
{
    mapping groups;

    user = lower_case(user);
    groups = filter(GROUP_D->get_group_data(),
		    (: member_array($(user), $2) != -1 :));

    if ( !sizeof(groups) )
	printf("%s doesn't belong to any groups.\n", user);
    else
	printf("%s belongs to: %s.\n", user,
	       implode(sort_array(keys(groups), 1), ", "));
}

private nomask void add_user_to_group(string user, string group)
{
    user = lower_case(user);
    group = lower_case(group);

    if ( !GROUP_D->get_group_data()[group] )
    {
	printf("There is no group named '%s'.\n", group);
    }
    else
    {
	GROUP_D->add_group_member(group, user);

	printf("Added %s to group %s.\n", user, group);
    }
}

private nomask void remove_user_from_group(string user, string group)
{
    user = lower_case(user);
    group = lower_case(group);

    GROUP_D->remove_group_member(group, user);

    printf("Removed %s from group %s.\n", user, group);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list groups",
	    action : (: list_groups :)),
	new(class command_info,
	    key : "s",
	    proto : "[group]",
	    desc : "show a group's members",
	    args : ({ "Show members of which group? " }),
	    action : (: show_group_members :)),
	new(class command_info),
	new(class command_info,
	    key : "c",
	    proto : "[group]",
	    desc : "create a group",
	    args : ({ "Name for the new group? " }),
	    who : "[Mudlib:]",
	    action : (: create_group :)),
	new(class command_info,
	    key : "d",
	    proto : "[group]",
	    desc : "delete a group",
	    args : ({ "Delete which group? " }),
	    who : "[Mudlib:]",
	    action : (: delete_group :)),
	new(class command_info),
	new(class command_info,
	    key : "u",
	    proto : "[user]",
	    desc : "show user's groups ",
	    args : ({ "Show groups for which user? " }),
            action : (: show_users_groups :)),
	new(class command_info,
	    key : "a",
	    proto : "[user] [group]",
	    desc : "add user to a group",
	    who : "[Mudlib:]",
	    args : ({ "Add which user? ", "Add '%s' to which group? " }),
	    action : (: add_user_to_group :)),
	new(class command_info,
	    key : "r",
	    proto : "[user] [group]",
	    desc : "remove a user from a group",
	    who : "[Mudlib:]",
	    args : ({ "Remove which user? ", "Remove '%s' from which group? " }),
	    action : (: remove_user_from_group :))
    });
}
