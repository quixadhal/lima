/* Do not remove the headers from this file! see /USAGE for more info. */

/* Created by Ranma@Koko Wa 951001 */

/* This command will show you your current exec path */

#include <mudlib.h>

inherit CMD;

private void main() 
{
    string * paths = this_body()->query_shell_ob()->get_variable("path");

    write(iwrap(implode(paths,
			(: $1 + $2[0..<2] + ", " :),
			"Your current path is: ")[0..<3]) + "\n");
}
