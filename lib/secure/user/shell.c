/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** shell.c -- shell handling for the user
**
** 950707, Rust: original player shell
** 950711, Deathblade: created.
** 960129, Deathblade: moved to the user ob
*/

#include <mudlib.h>

string query_userid();
void force_me(string what);
void update_translations();

/*
** The actuall shell object we're using
*/
nosave private object	shell_ob;


nomask object query_shell_ob()
{
    return shell_ob;
}

protected nomask void start_shell()
{
    if ( !shell_ob )
    {
	shell_ob = new(wizardp(query_userid()) ? WIZ_SHELL : PLYR_SHELL);
    }

    shell_ob->start_shell();
    update_translations();
}

protected nomask void stop_shell()
{
    if ( shell_ob )
	shell_ob->remove();
}

protected nomask void run_login_script()
{
    string login_file;

    if ( !wizardp(query_userid()) )
	return;

    // do .login stuff
    login_file = wiz_dir(this_object()) + "/.login";
    if ( file_size(login_file) > 0)
    {
	map_array(explode(read_file(login_file), "\n"), (: force_me :));
    }
}
