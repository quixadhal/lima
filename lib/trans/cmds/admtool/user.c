/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** user.c -- user administration options
**
** 950630, Deathblade: created.
*/

#include <mudlib.h>
#include <security.h>

void std_handler(string str);
varargs void modal_simple(function input_func, int secure);

static nomask void write_user_menu()
{
    write("Administration Tool: user administration\n"
	  "\n"
	  "    n <name> - nuke a user\n"
	  "    w <name> - wiz a user\n"
	  "    d <name> - dewiz a user\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void confirm_nuking(string who, string str)
{
    object o;

    str = lower_case(str);
    if ( str != "y" && str != "yes" )
    {
	write("Nuke aborted!\n");
	return;
    }

    if ( o = find_body(who) )
	o->quit();

    /* ### handle mail */
    rm(sprintf("/data/links/%c/%s.o", who[0], who));
    rm(sprintf("/data/players/%c/%s.o", who[0], who));
    printf("'%s' has been nuked.\n", capitalize(who));
}

private nomask void wiz_user(string who)
{
    object ob;
    string err;

    if ( !(ob = find_body(who)) )
    {
	printf("** '%s' must be logged on.\n", capitalize(who));
	return;
    }
    if ( SECURE_D->query_is_wizard(who) )
    {
	printf("** '%s' is already a wizard.\n", capitalize(who));
	return;
    }
    err = SECURE_D->create_wizard(who);
    if ( err )
    {
	printf("** Error: %s\n", err);
	return;
    }
    mkdir("/wiz/" + who);
    SECURE_D->set_protection("/wiz/" + who, 1, who + ":");
    ob->set_level(1);
    printf("'%s' is now a wizard.\n", capitalize(who));
}

private nomask void dewiz_user(string who)
{
    object ob;
    string err;

    if ( !(ob = find_body(who)) )
    {
	printf("** '%s' must be logged on.\n", capitalize(who));
	return;
    }
    if ( GROUP_D->adminp(ob) )
    {
	printf("** '%s' is an admin and cannot be dewizzed.\n",
	       capitalize(who));
	return;
    }
    if ( !SECURE_D->query_is_wizard(who) )
    {
	printf("** '%s' is not a wizard.\n", capitalize(who));
	return;
    }
    err = SECURE_D->delete_wizard(who);
    if ( err )
    {
	printf("** Error: %s\n", err);
	return;
    }
    SECURE_D->set_protection("/wiz/" + who, 1, -1);
    ob->set_level(0);
    printf("'%s' is no longer a wizard.\n", capitalize(who));
}

static nomask void receive_user_input(string str)
{
    string name;

    sscanf(str, "%s %s", str, name);

    if ( !name && member_array(str, ({ "n", "w", "d" })) != -1 )
    {
	write("** A name must be provided.\n");
	return;
    }
    else if ( name )
	name = lower_case(name);

    switch ( str )
    {
    case "n":
	printf("Are you sure you want to nuke '%s' ? ", capitalize(name));
	modal_simple((: confirm_nuking, name :));
	break;

    case "w":
	wiz_user(name);
	break;

    case "d":
	dewiz_user(name);
	break;

    case "?":
	write_user_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}
