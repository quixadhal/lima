/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void
main(mixed *arg) {
    object ob = arg[0];
    string func = arg[1];
    object shadow;
    
    // For now, until I get some real security on this
    if (!check_privilege(1))
	error("Insufficient privilege.\n");

    write_file("/secure/shadows/dbxtracer_tmp", func);
    shadow = new("/secure/shadows/dbxtracer_shadow");
    if (!(shadow->start_shadowing(ob)))
    {
	out("Failed.\n");
	destruct(shadow);
    }
    
    shadow->DBXTRACER_setup(this_user(), func);
}

	    
