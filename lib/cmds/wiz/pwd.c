/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz

#include <mudlib.h>

inherit CMD;

private void main()
{
    object ob = this_body()->query_shell_ob();

    printf("Dir:  %s\nFile: %s\n",
	   ob->get_variable("pwd") || "NONE",
	   ob->get_variable("cwf") || "NONE");
}

              
