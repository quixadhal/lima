/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <commands.h>

inherit SHELL;
inherit M_SHELLVARS;
inherit M_PROMPT;
inherit M_COMPLETE;
inherit M_GETOPT;
inherit M_REGEX;

private static string path;

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

private nomask string * expand_arguments(string* argv)
{
    int i;

    if ( this_user() != query_owner() )
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

private void print_argument(mixed argv)
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


// called when the value of the "path" variable changes
private void set_path(string arg) {
    if (!arrayp(arg))
	return;
    arg = filter(arg, (: stringp :));
    path = map(arg, (: evaluate_path :));
}

string query_path() {
    // New shells need to query this to get the saved value
    if (!path) set_path(get_variable("path"));
    return path;
}

private void fix_path()
{
    set_variable("path", ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/", 
			       CMD_DIR_PLAYER "/"}));
    printf("Ok, your path is fixed.\n");
}


private void show_shell_help()
{
    more_file("/help/wizard/shell");
}

private void create()
{
    if ( !clonep() )
	return;

    ::create();
    prompt::create();

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
    shell_bind_if_undefined("print",	(: print_argument :));
    shell_bind_if_undefined("?",	(: show_shell_help :));
    shell_bind_if_undefined("set",	(: cmd_set :));
    shell_bind_if_undefined("unset",	(: cmd_unset :));

    add_variable_hook("path", (: set_path :));
}
    
string query_shellname()
{
    return "wish (Lima wizard shell) v. 0.9";
}

static void execute_command(string * argv, string original_input)
{
    mixed	tmp;
    string	path = query_path();
    mixed 	cmd_info;
    string	channel_name;

    /* BEGINNING OF EXPANSION */

    if(sizeof(argv) == 1 && argv[0][0] == '$' && strlen(argv[0]) > 1)
    {
	print_variable(argv[0][1..]);
	return;
    }

    if(sizeof(argv) > 1)
	argv = argv[0..0] + map(argv[1..], (: expand_if_variable($1,1) :));
    
    // In some shells, this is the hook for doing username completion,
    // globbing, flag pre-parsing, etc...  In others, it's used to execute
    // code encased in ` `'s.
    argv = expand_arguments(argv - ({ "" }));
    if(!argv)
	return;

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

    // ### wtf is this?
    // Hmm, I might undo this one...  the only reason this is here is to 
    // allow \\$ to work right.  \$ can work right in other ways....
    argv = map(argv, (: stringp($1) ? replace_string($1, "\\$","$") : $1 :));

    // If there is a local shell command that matches our input, try to
    // execute it.
    evaluate(tmp=dispatch[argv[0]], argv);
    if(tmp)
	return;

    /* END OF EXPANSION */


    /* find and execute the given command */
    cmd_info = CMD_D->smart_arg_parsing(argv, path);
    if ( !intp(cmd_info) )
    {
	cmd_info[0]->call_main(cmd_info[2], cmd_info[1]);
	return;
    }
    if ( cmd_info > 0 )
    {
	if ( cmd_info != 1 )
	    printf("Found command is uncallable.\n");
	return;
    }
    if( cmd_info == -2)
    {
	printf("Unable to finish command.\n");
	return;
    }

    /* use the parser to try the command */
    if ( this_body()->do_game_command(original_input) )
	return;

    /* try a channel */
    channel_name = NCHANNEL_D->is_valid_channel(argv[0], this_body()->query_channel_list());
    if ( channel_name )
    {
	int chan_type = channel_name[0..4] == "imud_";

	NCHANNEL_D->cmd_channel(channel_name,
				implode(argv[1..], " "),
				chan_type);
	return;
    }

#ifdef AUTOMATIC_REHASH

    /* rehash the user's path's directories */
    CMD_D->cache(map(path, (: evaluate_path :)));

    /* retry command after the rehashing */
    cmd_info = CMD_D->smart_arg_parsing(argv, path);
    if ( !intp(cmd_info) )
    {
	cmd_info[0]->call_main(cmd_info[2], cmd_info[1]);
	return;
    }
    if ( cmd_info != 0 && cmd_info != -1 )
    {
	if ( cmd_info != 1 )
	    printf("Found command is uncallable.\n");
	return;
    }

#endif /* AUTOMATIC_REHASH */

    if(is_file(CMD_DIR_VERBS "/" + argv[0] + ".c"))
	write(this_body()->nonsense());
    else
	printf("I don't know the verb '%s'.\n", argv[0]);

    return;
}

void set_pwd(string fname)
{
    set_variable("pwd", fname);
}

void set_cwf(string fname)
{
    set_variable("cwf", fname);
}

void setup_for_save()
{
    ::setup_for_save();

    shellvars::setup_for_save();
}

static mixed what_prompt()
{
    return (: get_prompt :);
}

static nomask string query_save_path(string userid)
{
    return WSHELL_PATH(userid);
}
