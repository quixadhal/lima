/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** objfind.c
**
** Find the locations (environments) of instances of the given class
**
** 07-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>

inherit DAEMON;

private object * get_obs(string arg)
{
    return filter_array(objects(), (: base_name($1) == $(arg) :) );
}

int main(string arg)
{
    object * obs;

    if ( !check_privilege(1) )
	error("Must be an admin to use objpurge.\n");

    obs = get_obs(arg);
    printf("Removing %d objects of class %s\n", sizeof(obs), arg);
    obs->remove();

    obs = filter_array(obs, (: $1 :));  /* remove zeros */
    printf("Destructing %d objects of class %s\n", sizeof(obs), arg);
    map_array(obs, (: destruct :));

    return 1;
}
