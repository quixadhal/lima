/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

private void main(mixed *arg)
{
    string fname = arg[0];
    
    if ( !fname )
    {
	fname = wiz_dir(this_user());
	if(!is_directory(fname))
	    fname = "/help";
    }
    
    this_body()->query_shell_ob()->set_pwd(fname);

    outf("new cwd: %s\n", fname);
}

int help()
{
write("Usage: cd [directory]\n"
"Most file commands assume you're talking about your current working "
"directory, if you do not specify a full path.  This command sets your "
"current directory.  If no argument is given,  your home directory will "
"become your current working dir.  See also, ls, pwd, ed.\n"
);
}
