/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";


nomask string module_name() {
    return "colour";
}

nomask string module_key() {
    return "C";
}

nomask string module_user() {
    return "[Mudlib:daemons]";
}

nomask mixed module_priv() {
    return "Mudlib:daemons";
}

private void receive_remove_colour(string key) {
    ANSI_D->remove_default_colour(key);
    write("Done.\n");
}

private void receive_add_colour(string key, string value) {
    ANSI_D->add_default_colour(key, value);
    write("Done.\n");
}

private void list_colours() {
    mapping tmp;
    tmp = ANSI_D->defaults();
    write(implode(map(keys(tmp), (: lower_case :)), ", ") + "\n\n");
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list default colours",
	    action : (: list_colours :)),
	new(class command_info,
	    key : "a",
	    proto : "[colour] [value]",
	    desc : "add default colour",
	    args : ({ "Colour code? ", "Value? " }),
	    action : (: receive_add_colour :)),
	new(class command_info,
	    key : "r",
	    proto : "[colour]",
	    desc : "remove default colour",
	    args : ({ "Remove which colour? " }),
	    action : (: receive_remove_colour :))
    });
}
