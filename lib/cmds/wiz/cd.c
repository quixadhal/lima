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
    
    this_user()->query_shell_ob()->set_pwd(fname);
    outf("new cwd: %s\n", fname);
}
