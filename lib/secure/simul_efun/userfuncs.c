/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>
#include <security.h>

string base_name(mixed val);

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

    return u ? u->query_body() : 0;
}


nomask int wizardp(mixed m)
{
    if ( objectp(m) )
	m = m->query_userid();

    if ( stringp(m) )
	return SECURE_D->query_is_wizard(m);

    return 0;
}

nomask int adminp(mixed m)
{
    if ( objectp(m) )
	m = m->query_userid();

    return member_array(m, SECURE_D->query_domain_members("admin")) != -1;
}
