/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "security";
}

nomask string module_key() {
    return "s";
}

private nomask void privilege_owners()
{
    string * owners = SECURE_D->query_privilege_owners();

    more(sprintf("Owners:\n%-#79s\n", implode(sort_array(owners, 1), "\n")));
}

private nomask void receive_privilege_for_show(string priv)
{
    int n;
    mapping privmap;

    n = member_array(':', priv);
    if ( n == -1 )
    {
	string * privnames;
	string privname;

	privmap = SECURE_D->query_privilege_list(priv);
	if ( !privmap )
	{
	    write("** There is no such privilege.\n");
	    return;
	}

	privnames = sort_array(keys(privmap), 1);
	foreach ( privname in privnames )
	{
	    if ( sizeof(privmap[privname]) )
		write("[" + privname + "] -> " +
		  implode(privmap[privname], ", ") + "\n");
	    else
		write("[" + privname + "]\n");
	}
    }
    else
    {
	privmap = SECURE_D->query_privilege_list(priv[0..n-1]);
	if ( !privmap || !privmap[priv] )
	{
	    write("** There is no such privilege.\n");
	    return;
	}

	if ( sizeof(privmap[priv]) )
	    write("[" + priv + "] -> " + implode(privmap[priv], ", ") + "\n");
	else
	    write("[" + priv + "]\n");
    }
}

private nomask void receive_privilege_for_define(string priv)
{
    /* checks for admin/owner... */
    if ( write_error(SECURE_D->define_privilege(priv)) )
	return;

    printf("Privilege '%s' has been defined.\n", priv);
}

private nomask void receive_privilege_for_undefine(string priv)
{
    /* checks for admin/owner... */
    if ( write_error(SECURE_D->undefine_privilege(priv)) )
	return;

    printf("Privilege '%s' has been undefined.\n", priv);
}

private nomask void receive_privilege_for_add(string user, string priv)
{
    /* checks for admin/owner... */
    if ( write_error(SECURE_D->extend_access(priv, user)) )
	return;

    printf("Privilege '%s' has been added to '%s'.\n", user, priv);
}

private nomask void receive_privilege_for_remove(string user, string priv)
{
    /* checks for admin/owner... */
    if ( write_error(SECURE_D->restrict_access(priv, user)) )
	return;

    printf("Privilege '%s' has been removed from '%s'.\n", user, priv);
}

private nomask void remap_tree(mapping result, mapping tree, string path)
{
    string subdir;
    mixed * data;

    foreach ( subdir, data in tree )
    {
	string fullpath = path + "/" + subdir;

	if ( data[0] != -1 )	/* cleared privs */
	{
	    result[fullpath] = "" + data[0];
	}

	if ( data[1] )
	    remap_tree(result, data[1], fullpath);
    }
}

private nomask void list_dir_privs(string arg)
{
    mapping r_tree;
    mapping w_tree;
    string dir;
    string priv;
    string output;

    if( !is_directory( arg ))
    {
	write( "Invalid directory.\n");
	return;
    }
    arg = evaluate_path(arg);
    if ( arg == "/" )
	dir = arg = "";
    else
	dir = arg[0..strsrch(arg, "/", -1)-1];

    r_tree = ([ ]);
    w_tree = ([ ]);

    remap_tree(r_tree, SECURE_D->query_access_tree(arg, 0), dir);
    remap_tree(w_tree, SECURE_D->query_access_tree(arg, 1), dir);

    foreach ( dir, priv in r_tree )
    {
	if ( w_tree[dir] )
	    w_tree[dir] += sprintf("   (read: %s)", priv);
	else
	    w_tree[dir] = sprintf("(read: %s)", priv);
    }

    output = implode(({ "" }) + sort_array(keys(w_tree), 1),
      (: $1 + $2 + "   " + $(w_tree)[$2] + "\n" :)) + "\n";
    more(output);
}

private nomask void perform_set_dir_priv(int rw, string dir, mixed priv)
{
    dir = evaluate_path(dir);

    if ( priv == "0" )
	priv = 0;
    else if ( priv == "1" )
	priv = 1;

    /* checks for parent dir priv... */
    if ( write_error(SECURE_D->set_protection(dir, rw, priv)) )
	return;

    printf("Directory '%s' has been set to '%s'.\n", dir, priv);
}

private nomask void perform_clear_dir_priv(string dir)
{
    dir = evaluate_path(dir);

    /* checks for parent dir priv... */
    if ( SECURE_D->query_protection(dir + "/foo", 0) )
	if ( write_error(SECURE_D->set_protection(dir, 0, -1)) )
	    return;

    if ( SECURE_D->query_protection(dir + "/foo", 1) )
	if ( write_error(SECURE_D->set_protection(dir, 1, -1)) )
	    return;

    printf("The privileges on '%s' have been cleared.\n", dir);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "p",
	    desc : "privileges",
	    action : (: privilege_owners :)),
	new(class command_info,
	    key : "s",
	    proto : "[priv]",
	    desc : "show sub-privileges",
	    who : "[admin/owner]",
	    args : ({ "Show information for which privilege? " }),
	    action : (: receive_privilege_for_show :)),
	new(class command_info,
	    key : "n",
	    proto : "[priv]",
	    desc : "define new privilege",
	    who : "[admin/owner]",
	    args : ({ "Define which privilege? " }),			
	    action : (: receive_privilege_for_define :)),
	new(class command_info,
	    key : "u",
	    proto : "[priv]",
	    desc : "undefine privilege",
	    who : "[admin/owner]",
	    args : ({ "Undefine which privilege? " }),
	    action : (: receive_privilege_for_undefine :)),
	new(class command_info), // blank line
	new(class command_info,
	    key : "a",
	    proto : "[priv [priv]]",
	    desc : "add a privilege to another",
	    who : "[admin/owner]",
	    args : ({ "Adding which user (to a priv) ? ", "Add '%s' to which privilege? " }),
	    action : (: receive_privilege_for_add :)),
	new(class command_info,
	    key : "d",
	    proto : "[priv [priv]]",
	    desc : "delete a privilege from another",
	    who : "[admin/owner]",
	    args : ({ "Remove which user (from a priv) ? ", "Remove '%s' from which privilege? " }),
	    action : (: receive_privilege_for_remove :)),
	new(class command_info), // blank line
	new(class command_info,
	    key : "l",
	    proto : "[dir]",
	    desc : "list directory protections",
	    args : ({ "Which directory? " }),
	    action : (: list_dir_privs :)),
	new(class command_info,
	    key : "w",
	    proto : "[dir priv]",
	    desc : "set a directory's write protection",
	    who : "[parent priv]",
	    args : ({ "Set which directory's write privilege? ", "Set '%s' to what privilege? " }),
	    action : (: perform_set_dir_priv, 1 :)),
	new(class command_info,
	    key : "r",
	    proto : "[dir priv]",
	    desc : "set a directory's read protection",
	    who : "[admin]",
	    args : ({ "Set which directory's read privilege? ", "Set '%s' to what privilege? " }),
	    action : (: perform_set_dir_priv, 0 :)),
	new(class command_info,
	    key : "c",
	    proto : "[dir]",
	    desc : "clear a directory's protection",
	    who : "[parent priv]",
	    args : ({ "Clear which directory's privileges? " }),
	    action : (: perform_clear_dir_priv :)),
    });
}
