/* Do not remove the headers from this file! see /USAGE for more info. */

/* Created by Ranma@Koko Wa 951001 */

/* This command will remove a path from your current exec path */

#include <mudlib.h>

inherit CMD;

private void main(string * arg) 
{
    string path = arg[0] + "/";
    string * paths;
    object shell_ob = this_user()->query_shell_ob();

    paths = shell_ob->query_path();

    if ( member_array(path, paths) == -1 )
    {
	out("The path does not exist in your current path\n");
	return;
    }

    paths -= ({ path });
    shell_ob->set_variable("path", paths);

    out(implode(paths,
			(: $1 + $2[0..<2] + ", " :),
			"Your path is now: ")[0..<3] + "\n");
}
