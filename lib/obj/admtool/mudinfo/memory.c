/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "memory";
}

nomask string module_key() {
    return "M";
}

private void obj_and_prog() {
    mapping info = ([]);
    array k;
    string ret;
    
    foreach (object o in objects())
	info[file_name(o)] = memory_info(o);
    
    k = keys(info);
    k = sort_array(k, (: $(info)[$2] - $(info)[$1] :));

    ret = "Note: this is an upper bound, since it neglects sharing\n\n";
    
    foreach (string name in k)
	ret += sprintf("%-70.70s:%i\n", name, info[name]);
    
    more(ret);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "o",
	    desc : "objects and programs",
	    action : (: obj_and_prog :))
	    });
}
