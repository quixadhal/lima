/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** objcount.c
**
** Provide counts of instantiated objects on a per-class basis.
**
** 07-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>

inherit CMD;

mapping counts;

private void main(string arg)
{
    counts = ([ ]);

    map_array(objects( (: clonep :) ), (: counts[base_name($1)]++ :) );

    this_user()->more(sprintf("%O\n", counts));

    counts = 0;
}
