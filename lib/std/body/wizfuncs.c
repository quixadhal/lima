/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wizfuncs.c -- wizard functions for the body
**
** 950820, Deathblade: created.
*/

object query_link();

string query_home()
{
    string home;

    home = wiz_dir(this_object()) + "/workroom";
    if ( file_size(home + ".c") > 0 )
	return home;

    return "CLONE";
}

nomask void wiz_switch_body(string obname)
{
    query_link()->switch_body(obname, 0);

}

