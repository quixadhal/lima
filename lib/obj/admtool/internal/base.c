/* Do not remove the headers from this file! see /USAGE for more info. */

inherit M_ACCESS;
inherit M_INPUT;

// Most of these fields can be left blank (i.e. 0) if irrelevant or unwanted.
// key == 0 means "leave a blank line"
// proto == 0 means no prototype
// priv == 0 means no restriction
// who == 0 means no info
// args == 0 means no arguments
class command_info {
    string key;      // A one character string giving the key to be pressed to
		     // invoke this command.
    string proto;    // A short [12 char or less] summary of the args, if any
    string desc;     // A short [~40 chars] description of the command
    string priv;     // priv required to use this command
    string who;      // A short [~10 chars] description of who can use the
		     // command
    array args;      // An array of prompts to use to ask for arguments not
		     // given on the line.  Max == 2.
    function action; // The function to be called.
}

// Overload these
string module_name();
string module_key();
class command_info array module_commands();

mixed module_priv() {
    return 0;
}

// our variables
private string prompt;
private class command_info array commands;
private object parent;

private nomask string parent_name() {
    string path = base_name();
    int idx;

//### should be OB_ADMTOOL or something...
    // the top level object has no parent
    if ( path == "/obj/admtool/admtool2" )
	return 0;

    idx = strsrch(path, "/", -1);
    path = path[0..idx-1];

    // anything in /obj/admtool should have admtool2 as the parent
    if ( path == "/obj/admtool")
	return "/obj/admtool/admtool2";

    return path;
}

static void heading() {
    write("Administration Tool: " + module_name() + " administration\n\n");
}

void write_menu() {
    int maxlen = 0;	/* max length of "proto" field */

    heading();

    foreach (class command_info comm in commands)
	if ( comm->proto && sizeof(comm->proto) > maxlen )
	    maxlen = sizeof(comm->proto);

    foreach (class command_info comm in commands) {
	string desc = comm->desc;
	
	if (comm->who)
	    desc = desc + repeat_string(" ", 70 - maxlen - strlen(desc) - strlen(comm->who)) + comm->who;
	
	if (comm->key)
	    printf("  %1s %-*s - %s\n", comm->key, maxlen, comm->proto || "", desc);
	else
	    write("\n");
    }
    write("\n");
}

private nomask void do_main_menu() {
    modal_pop();
    parent->write_menu();
    destruct();
}

nomask void do_quit() {
    modal_pop();
    if (parent) parent->do_quit();
    destruct();
}

static nomask void do_help() {
    write_menu();
}

static nomask int write_error(string err) {
    if (err) {
	write("Error: " + err + ".\n");
	return 1;
    }
    return 0;
}

static class command_info array defaults() {
    return  ({
	    new(class command_info), // blank line
	    new(class command_info, key : "m", desc : "main menu", 
		action : (: do_main_menu :)),
	    new(class command_info, key : "q", desc : "quit", 
		action : (: do_quit :)),
	    new(class command_info, key : "?", desc : "help", 
		action : (: do_help :))
	});
}

// setup
void begin_menu();

void create() {
    set_privilege(1);
    
    // abstract class 
    if (strsrch(base_name(), "internal") != -1) return;
    
    commands = module_commands() + defaults();
    
    prompt = "(AdmTool:" + module_name() + ") [" + implode(commands, 
	      (: ((class command_info)$2)->key ? 
	       $1 + ((class command_info)$2)->key : $1 :), "") + "] > ";

    if (clonep()) {
	if (module_priv() && !check_privilege(module_priv())) {
	    write("Permission denied: need priv " + module_priv() + "\n");
	    return;
	}
	
	begin_menu();
    }
}

private void handle_input(string str) {
    string arg;

    if (strlen(str) > 1) {
	if (str[1] != ' ') {
	    write("** Format is: <option> <argument>\n");
	    write_menu();
	    return;
	}
	arg = str[2..];
	str = str[0..0];
    } else if (str == "") {
	if (parent)
	    str = "m";
	else
	    str = "q";
    }

    foreach (class command_info comm in commands) {
	if (str == comm->key) {
	    if (comm->priv && !check_privilege(comm->priv)) {
		printf("Permission denied; need privilege '%s'.\n",
		       comm->priv);
		return;
	    }
	    switch (sizeof(comm->args)) {
	    case 0:
		if (arg) {
		    write("** No argument required.\n");
		    return;
		}

		evaluate(comm->action);
		return;
	    case 1:
		input_one_arg(comm->args[0], comm->action, arg);
		return;
	    case 2:
		input_two_args(comm->args[0], comm->args[1], comm->action, arg);
		return;
	    default:
		error("No support for >2 args\n");
	    }
	}
    }
    
    write("** Unknown option (? for help)\n");
}

static nomask void do_modal_func() {
    modal_func((: handle_input :), prompt);
}

void begin_menu() {
    if (parent_name()) {
	parent = previous_object();

	if (base_name(parent) != parent_name())
	    error("Illegal call to begin_menu()\n");
    }
    
    write_menu();

    modal_push((: handle_input :), prompt);
}
