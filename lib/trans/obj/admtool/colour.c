/* Do not remove the headers from this file! see /USAGE for more info. */

/* Beek */

void std_handler(string str);
varargs void modal_func(function input_func, mixed prompt_func, int secure);
void do_one_arg(string arg_prompt, function fp, string arg);
void do_two_args(string arg1_prompt, string arg2_prompt,
		 function fp, string arg);

#define PROMPT_MISC	"(AdmTool:colour) [hmq?] > "

private nomask void write_colour_menu() {
    write(@END
Administration Tool: Miscellaneous administration

    l        - list default colours
    a        - add default colour
    r        - remove default colour

    m        - main menu
    q        - quit
    ?        - help
END);
}

private void receive_remove_colour(string key) {
    ANSI_D->remove_default_colour(key);
    write("Done.\n");
}

private void receive_add_colour(string key, string value) {
    ANSI_D->add_default_colour(key, value);
    write("Done.\n");
}

private nomask void receive_colour_input(string str) {
    mapping tmp;
    string arg;

    sscanf(str, "%s %s", str, arg);
    
    switch (str) {
    case "l":
	tmp = ANSI_D->defaults();
	write(implode(map(keys(tmp), (: lower_case :)), ", ") + "\n\n");
	break;

    case "a":
	do_two_args("colour? ", "value? ", (: receive_add_colour :), arg);
	break;

    case "r":
	do_one_arg("colour? ", (: receive_remove_colour :), arg);
	break;
	
    case "?":
	write_colour_menu();
	break;
	
    default:
	std_handler(str);
	break;
    }
}

static nomask nomask void begin_colour_menu() {
    modal_func((: receive_colour_input :), PROMPT_MISC);
    write_colour_menu();
}

