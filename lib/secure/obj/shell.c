/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <driver/origin.h>

inherit M_INPUT;
inherit M_HISTORY;
inherit M_ALIAS;
inherit M_SHELLFUNCS;
inherit M_SAVE;
inherit M_SCROLLBACK;

private static object owner;

void execute_command(string * argv, string original_input);
string query_shellname();

static function arg_to_words_func = (: explode($1," ") :);

void remove()
{
    if ( origin() != ORIGIN_LOCAL && owner && previous_object() != owner )
	error("illegal attempt to remove shell object\n");

    destruct();
}

void setup_for_save()
{
    /*
    ** This object has no variables to save, but many of the moduless do,
    ** so we must dispatch to them.
    */
    alias::setup_for_save();
    shellfuncs::setup_for_save();
}

static void shell_input(mixed input)
{
    mixed argv;
    string original_input;

    if(input == "") return;
    if(input == -1)
    {
	remove();
	return;
    }

    /*
    ** ### WORK IN PROGRESS HERE
    **
    ** Defined from the BASH shell
    **
    ** Evaluation order:
    ** - history expansion
    ** - store in history
    ** - alias expansion
    ** - shell-defined expansion
    **   - TENTATIVELY: wish does: left-to-right: brace, tilde, parameter,
    **     variable, command, arithmetic, then word splitting, then
    **     pathname expansion
    ** - execution
    **
    ** If the command uses parsed arguments (argv syntax), then the only
    ** expansion that can change the number of arguments are: brace
    ** expansion, word splitting, and pathname expansion.  An exception
    ** is the expansion of $@.
    */

    // is this history manipulation?
    if(input[0] == HISTORY_CHAR)
    {
	input = history_command(input);
	if(!input) return;
    }

    add_history_item(input);

    // convert input into words
    argv = evaluate(arg_to_words_func, input);
    // argv = map(argv, (: trim_spaces :));
    if (!sizeof(argv))
	return;

    // alias expansion... a leading \ ignores alias expansion
    if(strlen(argv[0]) > 1 && argv[0][0] == '\\')
    {
	argv[0] = argv[0][1..];
    }
    else
    {
	mixed tmp = expand_alias(argv);

	argv = stringp(tmp) ? evaluate(arg_to_words_func, tmp) : tmp;
    }

    // We want to do this here so that alias expansion catches for souls.
    original_input = implode(argv," ");


    execute_command(argv, original_input);
}

private void cmd_exit()
{
    if(modal_stack_size() == 1)
    {
//### I think we could just issue the quit command rather than force it
	this_user()->force_me("quit");
	return;
    }
    printf("Exiting %s\n", query_shellname());
    modal_pop();
    remove();
}

void create()
{
    if ( !clonep() )
	return;

    alias::create();
    history::create();
}

/*
** This function is used internally to prepare a shell for operation.
** Subclasses will typically override to set up bindings and variables
** with shell_bind_if_undefined() or set_if_undefined(), respectively.
*/
static void prepare_shell()
{
    shell_bind_if_undefined("alias",	(: cmd_alias :));
    shell_bind_if_undefined("unalias",	(: cmd_remove_alias($1,1) :));
    shell_bind_if_undefined("history",	(: history_command :));
    shell_bind_if_undefined("scrollback", (: cmd_scrollback :));
//    shell_bind_if_undefined("exit",	(: cmd_exit :));
}

static mixed what_prompt()
{
    return "> ";
}

void start_shell()
{
    if ( !owner )
	owner = this_body();

    if ( owner != this_body() || previous_object() != owner )
	error("illegal attempt to take over shell\n");

    modal_push((: shell_input :), what_prompt());

    prepare_shell();
}


nomask object query_owner()
{
    return owner;
}
