/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// Removes the objects that would be returned from objfind
// See: objfind


/*
** objpurge.c
**
** Remove instances od the given class
**
** 07-Nov-94. Created. Deathblade.
**
** Modified from objfind.c by someone at sometime
**
*/

#include <mudlib.h>

inherit CMD;

private object * get_obs(string arg)
{
    return filter_array(objects(), (: base_name($1) == $(arg) :) );
}

private void main(string arg)
{
    object * obs;

    if ( !check_privilege(1) )
	error("Must be an admin to use objpurge.\n");

    obs = get_obs(arg);
    outf("Removing %d objects of class %s\n", sizeof(obs), arg);
    obs->remove();

    obs = filter_array(obs, (: $1 :));  /* remove zeros */
    outf("Destructing %d objects of class %s\n", sizeof(obs), arg);
    map_array(obs, (: destruct :));
}
