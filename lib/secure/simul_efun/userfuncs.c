/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>
#include <security.h>

string base_name(mixed val);

nomask object *bodies()
{
    return users()->query_body() - ({ 0 });
}

nomask object this_body()
{
    object u = this_user();

    return u ? u->query_body() : 0;
}


varargs nomask object find_user(string str, int even_linkdead)
{
    object *choices;

    if ( even_linkdead )
	choices = children(USER_OB);
    else
	choices = users();
    choices = filter(choices, (: $1->query_userid() == $(str) :));

    if ( sizeof(choices) )
	return choices[0];

    return 0;
}


varargs nomask object find_body(string str, int even_linkdead)
{
    object u = find_user(str, even_linkdead);

    if (u && (u = u->query_body())) return u;
    
    foreach (object b in bodies()) {
	if (b->query_nickname() == str)
	    return b;
    }
    
    return 0;
}


nomask int wizardp(mixed m)
{
    if ( !m )
	m = this_user()->query_userid();
    else if ( objectp(m) )
	m = m->query_userid();

    if ( stringp(m) )
	return SECURE_D->query_is_wizard(m);

    return 0;
}

nomask int adminp(mixed m)
{
    if ( !m )
	m = this_user()->query_userid();
    else if ( objectp(m) )
	m = m->query_userid();

    return member_array(m, SECURE_D->query_domain_members("admin")) != -1;
}


nomask int user_exists(string user)
{
    return USER_D->user_exists(user);
}


//:FUNCTION get_user_variable
// Get a variable value from the current user's shell object.
nomask mixed get_user_variable(string varname)
{
    object shell;

#define UNDEFINED_VALUE	([])[0]

    if ( !this_user() )
	return UNDEFINED_VALUE;
    shell = this_user()->query_shell_ob();
    if ( !shell )
	return UNDEFINED_VALUE;
    return shell->get_variable(varname);
}

//:Function set_this_user()
//Works like set_this_player().  It was added for consistancies sake.
nomask void set_this_user(object ob)
{
  set_this_player(ob);
}
