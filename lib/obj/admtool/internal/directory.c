/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "base";

class command_info array module_commands() {
    class command_info array ret = ({});

    if (file_size(base_name()) != -2)
	error("Could not find directory\n");
    
    foreach (string file in get_dir(base_name() + "/*.c")) {
	string key, name, who;
	
	file = base_name() + "/" + file;

	key = file->module_key();
	name = file->module_name();
	who = file->module_user();
	
	if (key)
	    ret += ({ 
		new(class command_info, key : key, desc : name,
		    action : (: clone_object, file :),
		    who : who)
		    });
    }

    return ret;
}

