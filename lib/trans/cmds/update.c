/* Do not remove the headers from this file! see /USAGE for more info. */

//      /bin/system/_update.c
//      from the Nightmare Mudlib
//      destructs an old version of a file and loads a new one
//      created by Sulam 911219
//      security changes by Descartes of Borg 930810
//      mixed with the TMI update command by Beek 9301119
//     ported to ZorkMud, Beek 5/13/94
//     also added some features from the previous ZorkMUD update
//     ok, so I ended up rewriting alot of it.
//     Made it able to deal with 3.2's deep-only inherit_list
//     Rust made it so that it calls the doc daemon after successfully
//     loading an object.

#include <mudlib.h>
#include <daemons.h>
inherit CMD;

int do_update(string file, int deep);

private void main(mixed *arg, mapping flags) {
    object *obs;
    object ob;
    string temp;
    int n,deep_up;
    string file;

// This is a quick hack to make update accept cfile*
    if(arg[0] && !stringp(arg[0]))
    {
      foreach(file in arg[0])
	{
	  catch(main(({file}), flags));
	}
      return;
    }

    if (flags["r"]) deep_up = 1;
    if (flags["R"]) deep_up = 2;
    if (flags["z"]) deep_up = 3;
    file = arg[0];
    if (!file) file = this_body()->query_shell_ob()->get_variable("cwf");
    if (!file || file == "here") file = "/"+file_name(environment(this_body()));
    file = evaluate_path(file);
    this_body()->query_shell_ob()->set_cwf( file );
    sscanf(file, "%s.c", file);
    if (file_size(file+".c")==-1) {
	printf("update: no such file.\n");
	return 0;
    }
    if(file_size(file) == -2 && file_size(file+".c") < 0 )
    {
	printf("update: file is a directory.\n");
	return 0;
    }
    if(file_size(file+".c"))  this_body()->query_shell_ob()->set_cwf(file+".c");
    obs = 0;
    if (ob = find_object(file)) {
	obs = all_inventory(ob);
	for (n = 0; n < sizeof(obs); n++)
	{
	    if (obs[n]->query_link() && interactive(obs[n]->query_link()))
		obs[n]->move(VOID_ROOM);
	    else {
		destruct(obs[n]);
	    }
	}
    }
    if (file[<2..<1] != ".c") file += ".c";
    if (do_update(file,deep_up) < time())
        write(file + ": No update necessary.\n");

    for (n=0; n<sizeof(obs); n++) {
	if (obs[n]) obs[n]->move(file);
    }
}

int do_update(string file, int deep)
{
    object ob;
    int master_flag;
    int i,n;
    string res;
    int tmp;
    int newest_inh = 0;
    
    if (deep == 3) {
	mixed *info = stat(file);
	ob = load_object(file);

	foreach (string fn in inherit_list(ob)) {
	    tmp = do_update(fn, (deep>1?deep:0));
	    if (tmp > newest_inh) newest_inh = tmp;
	}
	// the file itself.
	if (info[1] > newest_inh) newest_inh = info[1];

	// return if object isn't out of date.
	if (info[2] >= newest_inh)
	    return info[2];

	destruct(ob, 1);      // pass 1: we're coming back soon
    } else {
	ob = find_object(file);
        if (ob) {
	    if (deep)
		foreach (string fn in inherit_list(ob))
		    do_update(fn, (deep > 1 ? deep : 0));
	    destruct(ob, 1);      // pass 1: we're coming back soon
	}
    }
    load_object(file);
    if (file[0] != '/') file = "/" + file; // bug in inherit_list()
    write(file +": Updated and loaded.\n");
    return time();
}

int help()
{
    write(wrap(
	"Usage: update [-rRz] <filename>\n"
	"The update commands loads or reloads the specefied file.  If no "
	"argument is given, the last file the wizard was working on is used "
	"or the environment if there is no such file.  The update command "
	"only affects the master copy, not any clones that were loaded "
	"before.\nThe optional flags -r and -R will also update files "
	"inherited by the file.  update -R will update this object, the "
	"files that it inherits, any files that _those_ inherit, and so "
	"on.  update -r just updates the files inherited by the object.  "
	"update -z is the same as update -R, except that it only updates "
	"files which are out of date with respect to their source or "
	"objects they inherit.  Note that #includes are NOT checked.\n"));
    return 1;
}
