#include <commands.h>
#include <security.h>
#include <daemons.h>

inherit __DIR__ "internal/base";

class command_info array module_commands(string);

string our_task;
class command_info array our_defaults;


string module_name() {
    return "main";
}


void finish_add_task(string title, string desc) {
    string t_id = TASK_D->add_task(our_task, title, desc, 
				   this_user()->query_userid());
    
    if (!t_id) {
	write("** Error: Unable to add '"+title+"' below "+our_task+".\n");
	return;
    }

    write("Added " + t_id + "(" + title + ").\n");
    do_help();
}

nomask private void my_add_task(string title) {
    input_one_arg("Description: ",(: finish_add_task, title :) );
}


nomask private void my_complete_task(string task_id) {
    int result;
    
    if (our_task != 0)
      task_id = our_task + "." + task_id;

    result = TASK_D->complete_task(task_id);
    
    if (!result) {
	write("** Error: Unable to complete task " + task_id + "\n");
	return;
    }
    
    if (stringp(result)) {
	write("** Error: " + result + "\n");
	return;
    }
    
    write("Done.\n");
    do_help();
}


nomask private void my_remove_task(string task_id) {
    if (our_task != 0)
      task_id = our_task + "." + task_id;
    
    if (!TASK_D->remove_task(task_id)) {
	write("** Error: No such task: " + task_id + "\n");
	return;
    }
    
    write("Done.\n");
    do_help();
}


class command_info array module_commands() {
    class command_info array ret = ({ });
    int n = 1;

    if (our_task != 0) {
	array t = TASK_D->query_task(our_task);
	
	ret += ({
	    new(class command_info, desc : t[1] )
	});
    }
    
    foreach (array task in TASK_D->query_tasks(our_task)) {
	string key, name, status;
	
	key = sprintf("%d", n);
	
	if (key) {
	    string file = base_name();
	    string task_id = key;
	    
	    if (our_task != 0)
	      task_id = our_task + "." + task_id;
	    
	    name   = TASK_D->query_title(task_id);
	    status = TASK_D->query_status(task_id);
	    status = repeat_string(".", 12 - sizeof(status)) + status;
	    status = "[" + status + "]";
	    
	    ret += ({ 
		new(class command_info, key : key, desc : name,
		    action : (: clone_object, file, task_id :),
		    status : status)
	    });
	}
	
	n++;
    }

    ret += ({
	new(class command_info),
	
	new(class command_info,
	    key : "a",
	    proto : "[title]",
	    args : ({ "Title: " }),
	    action : (: my_add_task :),
	    desc : "add task"),
	    
	new(class command_info,
	    key : "c",
	    proto : "[id]",
	    args : ({ "Task ID: " }),
	    action : (: my_complete_task :),
	    desc : "complete task"),
	    
	new(class command_info,
	    key : "r",
	    proto : "[id]",
	    args : ({ "Task ID: " }),
	    action : (: my_remove_task :),
	    desc : "remove task"),
    });
    
    return ret;
}

protected void heading() {
    string task_str = "Task Tool";
    
    if (our_task) {
	task_str = "["+our_task+"] " + TASK_D->query_title(our_task);
    }
    
    write("\n%^TASK_HEADING%^"
	  "--- " + task_str + " ---\n"
	  "%^RESET%^\n");
}

protected class command_info array defaults() {
    return  our_defaults;
}


void create(string task_id)
{
    if ( clonep() )
    {
	our_task = task_id;
	
	our_defaults = ({
	    new(class command_info), // blank line
	});
	
	/* Only set up "previous menu" option if not top-level. */
	if (our_task != 0) {
	    our_defaults += ({
		new(class command_info, key : "m", desc : "previous menu", 
		    action : (: do_main_menu :)),
	    });
	}

	our_defaults += ({ 
	    new(class command_info, key : "q", desc : "quit", 
		action : (: do_quit :)),
	    new(class command_info, key : "?", desc : "help", 
		action : (: do_help :))
	});

	
        if ( file_name(previous_object()) != CMD_OB_TASKTOOL &&
	     base_name(previous_object()) != TASKTOOL )
	{
	    write("Illegal attempt to clone TASKTOOL\n");
            destruct(this_object());
            return;
        }

	::create();
    }
}

