/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";


nomask string module_name() {
    return "misc";
}

nomask string module_key() {
    return "M";
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "h",
	    desc : "rebuild help data",
	    action : (: HELP_D->rebuild_data() :))
    });
}
