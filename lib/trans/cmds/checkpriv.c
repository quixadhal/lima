/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** checkpriv.c -- check if you are operating at a particular privilege
**
** 950816, Deathblade: created.
*/

#include <mudlib.h>
#include <security.h>

inherit CMD;

private void main(mixed arg)
{
    if ( !arg )
    {
	write("Usage: checkpriv <privilege>\n");
	return;
    }

    arg = trim_spaces(arg);
    if ( arg == "1" )
	arg = 1;
    else if ( arg == "0" )
	arg = 0;

    if ( !SECURE_D->valid_privilege(arg) )
    {
	printf("'%s' is not a valid privilege.\n", arg + "");
	return;
    }

    if ( check_privilege(arg) )
	printf("Verified.  You have privilege '%s'\n", arg + "");
    else
	printf("Sorry.  You do not have privilege '%s'\n", arg + "");
}
