/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** bodyshell.c -- body's builtin shell
**
** 950707, Rust: original player shell
** 950711, Deathblade: Created.
*/

#include <mudlib.h>
#include <commands.h>

inherit SHELL;
inherit M_COMPLETE;

void do_game_command(string input);
object query_link();


/*
** The shell object filename to use.  0 means the builtin shell.
*/
private string		shell_fname;

/*
** If an external shell is being used, then this is it, along with its
** save information.
*/
static private object	shell_ob;
private string		shell_saved_data;

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
    return shell_ob || this_object();
}

void start_shell()
{
    if(previous_object() && previous_object() != query_link())
	return;
    if ( wizardp(query_link()) )
    {
	if ( !shell_fname )
	    shell_ob = clone_object(WIZ_SHELL, shell_saved_data);
	else
	    shell_ob = clone_object(shell_fname, shell_saved_data);

	shell_ob->start_shell();
    }
    else
    {
	shell::create(shell_saved_data);
	shell::start_shell();
    }

    if ( shell_saved_data )
    {
	if(!shell_ob)
	    load_from_string(shell_saved_data,0);
	else
	    catch(shell_ob->load_from_string(shell_saved_data, 0));
    }
}


static void restart_shell()
{
    if (wizardp(query_link()) && !shell_ob)
      {
	start_shell();
	return;
      }
    if ( shell_ob )
	shell_ob->start_shell();
    else
	shell::start_shell();
}

static nomask void prepare_to_save()
{
  if(!shell_ob)
    shell_saved_data = save_to_string();
  else
    shell_saved_data = shell_ob->save_to_string();
}

string query_shellname()
{
    return "[ Builtin Body Shell ]";
}

static
int execute_command(string * argv, string original_input)
{
  object winner;
  string argument;

  winner = CMD_D->find_cmd_in_path(argv[0], ({ CMD_DIR_PLAYER "/" }));
  if ( !winner )
  {
      //return 0;
      do_game_command(original_input);
      return 1;
  }

  if ( sizeof(argv) > 1 )
    argument = implode(argv[1..]," ");

  winner->call_main(argument);
  return 1;
}

private nomask string expand_one_argument(string arg)
{
    mixed expansion;

    if ( strlen(arg) <= 1 || arg[<1] != '*' )
	return arg;
    expansion = complete_user(arg[0..<2]);
    if ( stringp(expansion) )
	return expansion;
    return arg;
}

static string * expand_arguments(string* argv)
{
    return map(argv, (: expand_one_argument :));
}




