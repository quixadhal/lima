/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

private void main(mixed *arg)
{
    if ( !arg[0] )
    {
	arg = ({ wiz_dir(this_user()) });
	if(!is_directory(arg[0]))
	    arg = ({"/help"});
    }
    
    this_body()->query_shell_ob()->set_pwd(arg[0]);

    outf("new cwd: %s\n", arg[0]);
}

int help()
{
write(wrap(
"Usage: cd [directory]\n"
"Most file commands assume you're talking about your current working "
"directory, if you do not specify a full path.  This command sets your "
"current directory.  If no argument is given,  your home directory will "
"become your current working dir.  See also, ls, pwd, ed.\n"
));
}
