/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "domain";
}

nomask string module_key() {
    return "d";
}

private nomask void list_domains()
{
    string * domains;

    domains = SECURE_D->query_domains();
    if ( !sizeof(domains) )
    {
	write("There are no domains.\n");
    }
    else
    {
	printf("The following domains exist:\n%-#78s\n",
	       implode(sort_array(domains, 1), "\n") + "\n");
    }
}

private nomask void show_domain_info(string domain)
{
    string * members;
    string * lords;

    members = SECURE_D->query_domain_members(domain);
    if ( !members )
    {
	printf("There is no domain named '%s'\n", domain);
    }
    else
    {
	lords = SECURE_D->query_domain_lords(domain) || ({ });
	members -= lords;

	printf("Domain %s:\n", domain);

	switch ( sizeof(lords) )
	{
	case 0:
	    write("Lords:   (none)\n");
	    break;

	case 1:
	    printf("Lord:    %s\n", lords[0]);
	    break;

	default:
	    printf("Lords:   %s\n", implode(lords, ", "));
	    break;
	}

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

private nomask void create_domain(int no_domain_dir, string domain)
{
    domain = lower_case(domain);

    /* this checks for admins... */
    if ( write_error(SECURE_D->create_domain(domain)) )
	return;

    write("Domain created.\n");

    if ( !no_domain_dir )
    {
	string dirname = "/domains/" + domain;

	mkdir(dirname);
	printf("Created %s\n", dirname);

	printf("Setting protection level of %s\n", dirname);
	write_error(SECURE_D->set_protection(dirname,
					     1,
					     capitalize(domain + ":")));
    }
}

private nomask void delete_domain(string domain)
{
    SECURE_D->set_protection("/domains/" + domain, 1, -1);

    /* this checks for admins... */
    if ( write_error(SECURE_D->delete_domain(domain)) )
	return;

    write("Domain deleted.\n");
}

private nomask void show_users_domains(string user)
{
    string * domains;

    domains = SECURE_D->query_domains(user) || ({ });
    if ( !sizeof(domains) )
	printf("Wizard %s doesn't belong to any domain.\n", user);
    else
	printf("Wizard %s belongs to %s.\n", user,
	       implode(sort_array(domains, 1), ", "));
}

private nomask void add_user_to_domain(int lord, string user, string domain)
{
    /* this checks for lord privs... */
    if ( write_error(SECURE_D->add_domain_member(domain, user, lord)) )
	return;

    printf("Added %s %s %s.\n", user, lord ? "as a lord of" : "to", domain);
}

private nomask void remove_user_from_domain(string user, string domain)
{
    /* this checks for lord privs... */
    if ( write_error(SECURE_D->remove_domain_member(domain, user)) )
	return;

    printf("Removed %s from %s.\n", user, domain);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list domains",
	    action : (: list_domains :)),
	new(class command_info,
	    key : "s",
	    desc : "show domain information",
	    proto : "[domain]",
	    args : ({ "Show information on which domain? " }),
	    action : (: show_domain_info :)),
	new(class command_info),
	new(class command_info,
	    key : "c",
	    desc : "create a domain",
	    proto : "[domain]",
	    who : "[admin]",
	    args : ({ "Name for the new domain? " }),
	    action : (: create_domain, 0 :)),
	new(class command_info,
	    key : "C",
	    desc : "create abstract domain (no dir)",
	    proto : "[domain]",
	    who : "[admin]",
	    args : ({ "Name for the new domain? " }),
	    action : (: create_domain, 1 :)),
	new(class command_info,
	    key : "d",
	    desc : "delete a domain",
	    proto : "[domain]",
	    who : "[admin]",
	    args : ({ "Delete which domain? " }),
	    action : (: delete_domain :)),
	new(class command_info),
	new(class command_info,
	    key : "u",
	    desc : "show user's domains",
	    proto : "[user]",
	    args : ({ "Show information for which user? " }),
	    action : (: show_users_domains :)),
	new(class command_info,
	    key : "a",
	    desc : "add user to a domain",
	    who : "[lord]",
	    proto : "[user] [domain]",
	    args : ({ "Add which user? ", "Add '%s' to which domain? " }),
	    action : (: add_user_to_domain, 0 :)),
	new(class command_info,
	    key : "A",
	    desc : "add user as a lord of the domain",
	    who : "[lord]",
	    proto : "[user] [domain]",
	    args : ({ "Add which user as a lord? ", "Add '%s' to which domain? " }),
	    action : (: add_user_to_domain, 1 :)),
	new(class command_info,
	    key : "r",
	    desc : "remove a user from a domain",
	    who : "[lord]",
	    proto : "[user] [domain]",
	    args : ({ "Remove which user? ", "Remove '%s' from which domain? " }))
    });
}
