/* Do not remove the headers from this file! see /USAGE for more info. */

//#include <security.h>
#include <mudlib.h>
#include <commands.h>

inherit SHELL;
inherit M_COMPLETE;
//inherit M_ACCESS;
inherit M_GETOPT;
inherit M_REGEX;

int is_variable(string name);

private mixed evaluate_code(mixed code)
{
  string dir;
    
  code = reg_assoc(code,({"\\$[a-zA-Z0-9]+"}),({0}))[0];
  code = 
  map(code, (: (strlen($1) > 1 && $1[0] == '$' && is_variable($1[1..]))  ?
	     "(this_body()->query_shell_ob()->get_variable(\""+$1[1..]+"\"))" :
	     $1 :));
  code = implode(code, "");

  // don't chop access
  if (check_privilege(1))
    dir = "/trans/tmp";
  else if (file_size(dir = wiz_dir(this_user())) != -2)
    dir = "/tmp";

  if(is_file(wiz_dir(this_user())+"/eval.h"))
      return exec_code(code, dir, wiz_dir(this_user())+"/eval.h");
  else
      return exec_code(code, dir);
}

mixed* expand_arguments(string* argv)
{
  int i;

  if(this_body() != query_owner())
    error("get your own shell, asswipe!\n");

  for(i=0;i<sizeof(argv);i++)
    {

      if(strlen(argv[i])>2 && argv[i][0] == '`' && argv[i][<1] == '`')
	{
	  if(catch(argv[i] = evaluate_code(argv[i][1..<2])))
	    return 0;
	}
    }
  return argv;
}

private void
print_argument(mixed argv)
{
  mixed item;

  if(sizeof(argv) == 1)
    {
      printf("print `code`\n");
      return;
    }
  foreach(item in argv[1..])
    printf("%O ",item);
  write("\n");
}  


private void
fix_path()
{
  set_variable("path", ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/", 
			   CMD_DIR_PLAYER "/"}));
  printf("Ok, your path is fixed.\n");
}


private void
show_shell_help()
{
    more_file("/help/wizard/shell");
}

private void create()
{
  if ( !clonep() )
    return;

  ::create();

  set_privilege(1);

  /* set up to save variables through the M_SAVE api */
  setup_for_save();

  arg_to_words_func = (: argument_explode :);
}

static void prepare_shell()
{
    ::prepare_shell();

    set_if_undefined("path", ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/", 
				   CMD_DIR_PLAYER "/"}));
    set_if_undefined("pwd", "/");
    set_if_undefined("cwf", 0);

    shell_bind_if_undefined("resetpath", (: fix_path :));
    shell_bind_if_undefined("print", (: print_argument :));
    shell_bind_if_undefined("?", (: show_shell_help :));
}
    
string
query_shellname()
{
  return "wish (Lima wizard shell) v. 0.9";
}

static int
execute_command(string * argv, string original_input)
{
    string	path = get_variable("path");
    mixed 	cmd_info;

    /* find and execute the given command */
    cmd_info = CMD_D->smart_arg_parsing(argv, path);
    if ( !intp(cmd_info) )
    {
	cmd_info[0]->call_main(cmd_info[2], cmd_info[1]);
	return 1;
    }
    if ( cmd_info != 0 && cmd_info >= 0 )
    {
	if ( cmd_info != 1 )
	    printf("Found command is uncallable.\n");
	return -1;
    }
    if( cmd_info == -2)
      {
	printf("Unable to finish command.\n");
	return -1;
      }

    /* use the parser to try the command */
    if ( this_body()->do_game_command(original_input) )
	return 1;

    /* try a channel */

#ifdef AUTOMATIC_REHASH

    /* rehash the user's path's directories */
    CMD_D->cache(map(path, (: evaluate_path :)));

    /* retry command after the rehashing */
    cmd_info = CMD_D->smart_arg_parsing(argv, path);
    if ( !intp(cmd_info) )
    {
	cmd_info[0]->call_main(cmd_info[2], cmd_info[1]);
	return 1;
    }
    if ( cmd_info != 0 && cmd_info != -1 )
    {
	if ( cmd_info != 1 )
	    printf("Found command is uncallable.\n");
	return -1;
    }

#endif /* AUTOMATIC_REHASH */

	    if(is_file(CMD_DIR_VERBS "/" + argv[0] + ".c"))
	      write(this_body()->nonsense());
	    else
	      printf("I don't know the verb '%s'.\n", argv[0]);

    return 1;
}

void set_pwd(string fname)
{
    set_variable("pwd", fname);
}

void set_cwf(string fname)
{
    set_variable("cwf", fname);
}
