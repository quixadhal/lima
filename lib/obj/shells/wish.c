/* Do not remove the headers from this file! see /USAGE for more info. */

//#include <security.h>
#include <mudlib.h>
#include <commands.h>

inherit SHELL;
inherit M_COMPLETE;
//inherit M_ACCESS;
inherit M_SAVE;
inherit M_GETOPT;
inherit M_REGEX;

int is_variable(string name);

mixed evaluate_code(mixed code)
{
  code = reg_assoc(code,({"\\$[a-zA-Z0-9]+"}),({0}))[0];
  code = 
  map(code, (: (strlen($1) > 1 && $1[0] == '$' && is_variable($1[1..]))  ?
	     "(this_body()->query_shell_ob()->get_variable(\""+$1[1..]+"\"))" :
	     $1 :));
  code = implode(code, "");
  if(is_file(wiz_dir(this_user())+"/eval.h"))
    return exec(code, wiz_dir(this_user())+"/eval.h");
  else
    return exec(code);
}

mixed* expand_arguments(string* argv)
{
  int i;

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

void
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


void
fix_path()
{
  set_variable("path", ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/", 
			   CMD_DIR_PLAYER "/"}));
  printf("Ok, your path is fixed.\n");
}


void
show_shell_help()
{
  new(MORE_OB)->more_file("/help/wizard/shell");
}

void
create()
{
  if ( !clonep() )
    return;

  shell::create();

  set_privilege(1);

  /* set up to save variables through the M_SAVE api */
  setup_for_save();

  set_variable("path", ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/", 
			   CMD_DIR_PLAYER "/"}));
  set_variable("PROMPT", "? for shell help>");

  arg_to_words_func = (: argument_explode :);
  shell_bind("resetpath", (: fix_path :));
  shell_bind("print", (: print_argument :));
  shell_bind("?", (: show_shell_help :));
}


string
query_shellname()
{
  return "wish (Lima wizard shell) v. 0.9";
}

int
execute_command(string * argv, string original_input)
{
  mixed 	cmd_info;

  cmd_info = CMD_D->smart_arg_parsing(argv, get_variable("path"));
  if(intp(cmd_info))
    {
      if(!cmd_info || cmd_info == -1) 
	{
	  this_body()->do_game_command(original_input);
	  return 1;
	}
      if(cmd_info != 1)
	printf("Found command is uncallable.\n");
      return -1;
    }
  cmd_info[0]->main(cmd_info[2], cmd_info[1]);
  return 1;
}
