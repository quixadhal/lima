/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** objfind.c
**
** Find the locations (environments) of instances of the given class
**
** 07-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>

inherit CMD;

mapping envs;

private void main(string arg)
{
    object * obs;

    obs = objects( (: base_name($1) == $(arg) :) );

    envs = ([ ]);
    map_array(obs, (: envs[$1] = environment($1) :));

    more(sprintf("%O\n", envs));
}
