/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** sw_user.c -- switch the actual user
**
** 14-Jun-95.  Deathblade.  Created.
*/

#include <mudlib.h>
#include <security.h>
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
varargs mixed unguarded(mixed priv, function code);

void clear_input_stack();


private nomask int do_su(string old_userid, string new_userid)
{
    object body;
    string old_name;
    string new_name;

    write("\n");

    old_name = query_body()->query_name();

    /*
    ** The input stack needs to be cleared before we switch bodies so that
    ** the shell the body creates becomes the bottom object on the input 
    ** stack.  -- Rust
    */
    clear_input_stack();

    /*
    ** switch user, then switch body.  Note on switching user:  pass
    ** flag == 1 so that we do not blow away static variables (the
    ** old body).
    */
    restore_me(new_userid, 1);
    switch_body(0);

    /* alter privileges */
    if ( GROUP_D->adminp(this_object()) )
	set_privilege(1);
    else
	set_privilege(query_userid());
    
    body = query_body();
    new_name = body->query_name();
    if ( old_userid != new_userid )
    {
	tell_room(environment(body),
		  sprintf("%s has polymorphed into %s.\n",
			  old_name, new_name),
		  ({ body }) );
    }
    tell_object(this_object(), sprintf("Done. You are now %s.\n", new_name));
}

private nomask void confirm_valid_su(string old_userid,
				     string new_userid,
				     string arg)
{
    string pwd = USER_D->query_password(new_userid);

    if ( crypt(arg, arg) != pwd )
    {
	write("\nWrong password.\n");
	register_failure(sprintf("(su to %s)", new_userid));
	return;
    }

    do_su(old_userid, new_userid);
}

nomask void switch_user(string str)
{
    int is_admin = GROUP_D->adminp(this_object());
    string old_userid = query_userid();
    string new_userid = lower_case(str);

    if ( previous_object() != find_object(CMD_OB_SU) )
	return;

    new_userid = lower_case(str);
    if ( !SECURE_D->valid_name(new_userid) )
    {
	printf("'%s' is an invalid name.\n", str);
	return;
    }

    if ( new_userid != old_userid )
    {
	string linkpath = LINK_PATH(new_userid) + __SAVE_EXTENSION__;

	if( unguarded(1, (: file_size($(linkpath)) :)) <= 0 )
	{
	    write("No such user.\n");
	    return;
	}
    }

    if ( is_admin || new_userid == old_userid )
    {
	do_su(old_userid, new_userid);
	return;
    }

    printf("switching to: '%s'\n", new_userid);

    write("Password: ");
    modal_simple((: confirm_valid_su, old_userid, new_userid :), 1);
}
