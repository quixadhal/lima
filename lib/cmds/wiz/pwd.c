/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz

#include <mudlib.h>

inherit CMD;

private void main()
{
    outf("Dir:  %s\nFile: %s\n",
	 get_user_variable("pwd") || "NONE",
	 get_user_variable("cwf") || "NONE");
}
