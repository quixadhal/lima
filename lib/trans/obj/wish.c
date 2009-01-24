/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <commands.h>

inherit SHELL;
inherit M_SHELLVARS;
inherit M_PROMPT;
inherit M_COMPLETE;
inherit M_GETOPT;
inherit M_REGEX;

#define WIZ_CMD_DIRS ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/",\
	CMD_DIR_CREATE "/", CMD_DIR_PLAYER "/"})

#define ADMIN_CMD_DIRS ({CMD_DIR_NO_RESTRICT "/", CMD_DIR_RESTRICT "/",\
	CMD_DIR_CREATE "/", CMD_DIR_PLAYER "/", CMD_DIR_ADMIN "/" })

private nosave string path;
int	expand_vars;

void set_expand_vars(int i)
{
    expand_vars = i;
}

int is_variable(string name);
private mixed check_for_nuggets(string array args);

private void cmd_rehash()
{
    CMD_D->cache(map(path,(:evaluate_path:)));
    write("Okay.\n");
}

private mixed evaluate_code(mixed code)
{
    string dir;

    code = reg_assoc(code,({"\\$[a-zA-Z0-9]+"}),({0}))[0];

    code =
    map(code, (: (strlen($1) > 1 && $1[0] == '$' && is_variable($1[1..]))  ?
	"(get_user_variable(\""+$1[1..]+"\"))" :
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
    int i, n;

    if ( this_user() != query_owner() )
	error("get your own shell, asswipe!\n");

    n = sizeof(argv);
    for( i = 0; i < n; i++)
    {

	if(strlen(argv[i])>2 && argv[i][0] == '`' && argv[i][<1] == '`')
	{
	    if(catch(argv[i] = evaluate_code(argv[i][1..<2])))
		return 0;
	}
    }
    return argv;
}


// called when the value of the "path" variable changes
private void set_path(string *arg) {
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
    set_variable("path", WIZ_CMD_DIRS);
    if(query_owner() && adminp(query_owner()))
      set_variable("path", ADMIN_CMD_DIRS);
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

protected void prepare_shell()
{
    ::prepare_shell();

    set_if_undefined("path", WIZ_CMD_DIRS);

    if(query_owner() && adminp(query_owner()))
      set_if_undefined("path", ADMIN_CMD_DIRS);

    set_if_undefined("pwd", "/");
    set_if_undefined("cwf", 0);

    shell_bind_if_undefined("resetpath", (: fix_path :));
    shell_bind_if_undefined("?",	(: show_shell_help :));
    shell_bind_if_undefined("set",	(: cmd_set :));
    shell_bind_if_undefined("unset",	(: cmd_unset :));
    shell_bind_if_undefined("rehash",   (: cmd_rehash :));

    add_variable_hook("path", (: set_path :));
}

string query_shellname()
{
    return "wish (Lima wizard shell) v. 0.9";
}

varargs protected void execute_command(string original_input)
{
    string * argv = explode(original_input, " ");
    mixed	tmp;
    string	path = query_path();
    mixed 	cmd_info;
    string	channel_name;
    mixed	orig_argv;
    mixed	stdin_info;
    mixed	extra_args;
    mixed       nugget_info;
    mixed	remaining_implode_info;
    string	virgin_input;
    string      array implode_info;

    /* BEGINNING OF EXPANSION */
    tmp = evaluate(arg_to_words_func, implode(argv," "));
    argv = tmp[0];
    implode_info = tmp[1];

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
    argv = expand_arguments( argv - ({ "" }) - ({}));

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
	    set_variable(argv[0][1..], implode_by_arr(map(argv[2..], 
		  (: expand_if_variable :)),
		implode_info[2..]));
	return;
    }

    // Expand variables
    if(expand_vars)
	argv = map(argv, (: expand_if_variable :));

    // ### wtf is this?
    // Hmm, I might undo this one...  the only reason this is here is to 
    // allow \\$ to work right.  \$ can work right in other ways....
    argv = map(argv, (: stringp($1) ? replace_string($1, "\\$","$") : $1 :));

    // If there is a local shell command that matches our input, try to
    // execute it.
    if (!sizeof(argv)) return;
    evaluate(tmp=dispatch[argv[0]], argv, implode_info);
    if(tmp)
	return;

    /* END OF EXPANSION */

    orig_argv = argv;
    virgin_input = implode_by_arr(map(orig_argv[1..], 
	(:stringp($1)?$1:sprintf("%O",$1):)), implode_info[1..])[1..];

    nugget_info = check_for_nuggets(argv);
    argv = nugget_info[0];
    stdin_info = nugget_info[1];
    extra_args = nugget_info[2];
    remaining_implode_info = implode_info[sizeof(argv)..];
    implode_info = implode_info[1..(sizeof(argv)-1)]+({""});
    if(sizeof(implode_info) && (implode_info[0][0..0] == " "))
	implode_info[0] = implode_info[0][1..];


    /* find and execute the given command */
    cmd_info = CMD_D->smart_arg_parsing(argv, path, implode_info);
    if ( !intp(cmd_info) )
    {
	if(strlen(virgin_input) == strlen(original_input))
	    virgin_input = "";
	while(1)
	{
	    mixed tmp2;
	    cmd_info = cmd_info[0]->call_main(cmd_info[2], cmd_info[1], 
	      stdin_info, extra_args,
	      implode_info, 
	      remaining_implode_info,
	      virgin_input);
	    if(!cmd_info)
		return;
	    stdin_info = cmd_info[1];
	    implode_info = cmd_info[2];
	    nugget_info = check_for_nuggets(cmd_info[0]);
	    argv = nugget_info[0];
	    extra_args = nugget_info[2];
	    tmp2 = argv;
	    if(arrayp(extra_args))
		tmp2 += extra_args;
	    if(!sizeof(tmp2))
	    {
		write("error: pipe to nothing.\n");
		return;
	    }
	    virgin_input = implode_by_arr(tmp2[1..],  
	      implode_info[(sizeof(implode_info)-sizeof(tmp2))..]);
	    if(virgin_input[0] == ' ')
		virgin_input = virgin_input[1..];
	    remaining_implode_info = implode_info[sizeof(argv)..];
	    implode_info = implode_info[1..(sizeof(argv)-1)]+({""});
	    if(sizeof(implode_info) && (implode_info[0][0..0] == " "))
		implode_info[0] = implode_info[0][1..];
	    cmd_info = CMD_D->smart_arg_parsing(argv,path,implode_info);
	    if(intp(cmd_info))
	    {
		if (sizeof(argv))
		    printf("error: pipe to %s failed.\n", argv[0]);
		else
		    write("error: pipe to nothing.\n");
		return;
	    }
	}
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

    if (!sizeof(argv)) {
	printf("Nothing before pipe.\n");
	return;
    }

    if (this_body()) {
	/* use the parser to try the command */
	if ( this_body()->do_game_command(original_input) )
	    return;

	/* try a channel */
	channel_name = CHANNEL_D->is_valid_channel(orig_argv[0], this_user()->query_channel_list());
	if ( channel_name ) {
	    int chan_type = channel_name[0..4] == "imud_";
	    CHANNEL_D->cmd_channel(channel_name,
	      virgin_input,
	      chan_type);
	    return;
	}

	if(is_file(CMD_DIR_VERBS "/" + argv[0] + ".c"))
	    write(this_body()->nonsense());
	else
	    printf("I don't know the verb '%s'.\n", argv[0]);
    } else {
	if (is_file(CMD_DIR_VERBS "/" + argv[0] + ".c"))
	    write("Can't use verb with no body.\n");
	else
	    printf("I don't know the verb '%s'.\n", argv[0]);
    }
}

void set_pwd(string fname)
{
    set_variable("lastpwd", get_variable("pwd"));
    set_variable("pwd", fname);
}

void swap_pwd() {
    set_pwd(get_variable("lastpwd"));
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

protected mixed what_prompt()
{
    return (: get_prompt :);
}

protected nomask string query_save_path(string userid)
{
    return WSHELL_PATH(userid);
}

// Support for IO redirection
private mixed check_for_nuggets(string array args)
{
    int		i,j;
    string array	stdinstuff = 0;

    i = member_array("<",args);
    if(i != -1)
    {
	j = i+1;
	while((j < sizeof(args)) && (args[j] != ">") && (args[j] != ">>")
	  && (args[j] != "|") && (args[j] != "<")) j++;
	stdinstuff = args[i+1..(j-1)];
	args = args[0..(i-1)] + args[j..];
    }

    for(i=0;i<sizeof(args);i++)
    {
	if(stringp(args[i]))
	    switch(args[i])
	{
	case "|":
	case ">":
	case ">>":
	    return ({args[0..(i-1)], stdinstuff, args[i..]});
	default:
	    break;
	}
    }
    return ({args, stdinstuff, 0});
}


