/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** sw_user.c -- switch the actual user
**
** 14-Jun-95.  Deathblade.  Created.
*/

#include <mudlib.h>
#include <commands.h>

string query_userid();
string query_body_fname();
object query_body();

string query_userid();
varargs void restore_me(string some_name);
int matches_password(string str);

void switch_body(mixed new_body);

void register_failure(string addr);

void modal_simple(function input_func, int secure);

void set_privilege(mixed priv);		// from M_ACCESS

void clear_input_stack();


private nomask int do_su()
{
    object body;
    string old_name;
    string new_name;

    write("\n");
    old_name = this_body()->query_name();

// The input stack needs to be cleared before we switch bodies so that
// the shell the body creates becomes the bottom object on the input 
// stack.  -- Rust

    clear_input_stack();  
    switch_body(0);	/* create new body */

    /* alter privileges */
    if ( GROUP_D->adminp(this_object()) )
	set_privilege(1);
    else
	set_privilege(query_userid());
    
    body = query_body();
    new_name = body->query_name();
    if ( old_name != new_name )
    {
	tell_room(environment(body),
		  sprintf("%s has polymorphed into %s.\n",
			  old_name, new_name),
		  ({ body }) );
    }
    tell_object(this_object(), sprintf("Done. You are now %s.\n", new_name));
}

private nomask void confirm_valid_su(string old_user, string arg)
{
    if ( !matches_password(arg) )
    {
	write("\nWrong password.\n");
	register_failure(sprintf("(su by %s)", old_user));
	restore_me(old_user, 1);
	return;
    }

    do_su();
}

nomask void switch_user(string str)
{
    int is_admin = GROUP_D->adminp(this_object());
    string old_user = query_userid();

    if ( previous_object() != find_object(CMD_OB_SU) )
	return;

    str = lower_case(str);
    if ( str != old_user )
    {
	restore_me(str, 1);	/* don't trash the body or input stack */
	if ( str != query_userid() )
	{
	    write("No such user.\n");
	    restore_me(old_user, 1);
	    return;
	}
    }

    if ( is_admin || str == old_user )
    {
	do_su();
	return;
    }

    write("Password: ");
    modal_simple((: confirm_valid_su, old_user :), 1);
}
