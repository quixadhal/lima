/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** bodyshell.c -- body's builtin shell
**
** 950707, Rust: original player shell
** 950711, Deathblade: Created.
*/

#include <mudlib.h>

object query_link();


/*
** The shell object filename to use.  0 means the default shell.
*/
private string		shell_fname;

/*
** The saved information for the shell object
*/
private string		shell_saved_data;

/*
** The actuall shell object we're using
*/
static private object	shell_ob;


nomask string query_shell_fname()
{
    return shell_fname;
}
nomask void set_shell_fname(string fname)
{
    shell_fname = fname;

    /* ### switch shells now? */
}

nomask object query_shell_ob()
{
    return shell_ob;
}

static void start_shell()
{
    if ( !shell_ob )
    {
	string fname = shell_fname;

	if ( !fname )
	    fname = wizardp(query_link()) ? WIZ_SHELL : PLYR_SHELL;

	shell_ob = new(fname, shell_saved_data);

	if ( shell_saved_data )
	{
	    /* sometimes incompatibilities break the restore... :-( */
	    catch(shell_ob->load_from_string(shell_saved_data, 0));
	}
    }

    shell_ob->start_shell();
}

static nomask void prepare_to_save()
{
    if ( shell_ob )
	shell_saved_data = shell_ob->save_to_string();
}
