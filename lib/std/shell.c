/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_INPUT;
inherit M_HISTORY;
inherit M_ALIAS;
inherit M_SHELLVARS;
inherit M_SHELLFUNCS;
inherit M_SAVE;
inherit M_SCROLLBACK;
inherit M_PROMPT;

private static object owner;

void execute_command(string * argv, string original_input);
string query_shellname();

static function arg_to_words_func = (: explode($1," ") :);

static void remove()
{
  destruct(this_object());
}

void setup_for_save()
{
    /*
    ** This object has no variables to save, but many of the moduless do,
    ** so we must dispatch to them.
    */
    alias::setup_for_save();
    shellvars::setup_for_save();
    shellfuncs::setup_for_save();
}

static string*
expand_arguments(string* argv)
{
  return argv;
}

static void 
shell_input(mixed input)
{
  mixed argv, tmp;
  string original_input;
  if(input == "") return;
  if(input == -1)
    {
      remove();
      return;
    }

  add_history_item(input);

// is this history manipulation?
  if(input[0] == HISTORY_CHAR)
    {
      input = history_command(input);
      if(!input) return;
    }

// convert input into words
  argv = evaluate(arg_to_words_func, input);
//  argv = map(argv, (: trim_spaces :));

// alias expansion... a leading \ ignores alias expansion
  if(strlen(argv[0]) > 1 && argv[0][0] == '\\')
    {
      argv[0] = argv[0][1..];
    }
  else
    {
      tmp = expand_alias(argv);
      argv = stringp(tmp) ? evaluate(arg_to_words_func, tmp) : tmp;
    }
// We want to do this here so that alias expansion catches for souls.
  original_input = implode(argv," ");


if(sizeof(argv) == 1 && argv[0][0] == '$' && strlen(argv[0]) > 1)
  {
    print_variable(argv[0][1..]);
    return;
  }

  if(sizeof(argv) > 1)
    argv = argv[0..0] + map(argv[1..], (: expand_if_variable($1,1) :));
    
// In some shells, this is the hook for doing username completion, globbing, flag pre-parsing,
// etc...  In others, it's used to execute code encased in ` `'s.

  argv = expand_arguments(argv);
  if(!argv) return;
  argv -= ({ "" });
  if(wizardp(this_user()))
    argv = map(argv, 
	     (: (stringp($1)) ? replace_string($1,"\\`","`") :  $1 :));

// check for if this is a variable setting.
  if(sizeof(argv) > 2 && argv[1] == "=" && strlen(argv[0]) > 1 &&
     argv[0][0] == '$')
    {
      if(sizeof(argv) == 3)
	set_variable(argv[0][1..],expand_if_variable(argv[2]));
      else
	set_variable(argv[0][1..], implode(map(argv[2..], 
					     (: expand_if_variable :)),
					 " "));
      return;
    }

// Expand variables
  argv = map(argv, (: expand_if_variable :));

// Hmm, I might undo this one...  the only reason this is here is to 
// allow \\$ to work right.  \$ can work right in other ways....
  argv = map(argv, (: stringp($1) ? replace_string($1, "\\$","$") : $1 :));

// If there is a local shell command that matches our input, try to execute it.
  evaluate(tmp=dispatch[argv[0]], argv);
  if(tmp)
    return;
  if(!execute_command(argv, original_input))
    modal_pass(original_input); // Hmm, using original_input may not be
      // expected.  People might want their aliases and history cmds to
      // work, even though the command wasn't executed in the shell...
}

void start_shell()
{
  if(owner)
    return;
  owner = this_body();
  modal_push((: shell_input :), (: wizardp($(this_user())) ? get_prompt() : "> " :));
  history::create();
}

private void cmd_exit(){
  if(sizeof(this_user()->modal_stack_size()) == 1)
    {
      this_user()->force_me("quit");
      return;
    }
  printf("Exiting %s\n", query_shellname());
  this_user()->modal_pop();
  remove();
}

void create(string shell_saved_data)
{
  if(owner)
    return;
  alias::create();
  history::create();
  prompt::create();
  shell_bind("set",    (: cmd_set :));
  shell_bind("unset",  (: cmd_unset :));
  shell_bind("alias",  (: cmd_alias :));
  shell_bind("unalias", (: cmd_remove_alias($1,1) :));
  shell_bind("history", (: history_command :));
  shell_bind("scrollback", (: cmd_scrollback :));
  shell_bind("exit", (: cmd_exit :));
}


void reconnect()
{
  if(this_body() != owner)
    return;
  scrollback::reconnect();
}


nomask object get_owner()
{
  return owner;
}
