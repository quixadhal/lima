/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** objcount.c
**
** Provide counts of instantiated objects on a per-class basis.
**
** 07-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>

inherit DAEMON;

mapping counts;

int main(string arg)
{
    counts = ([ ]);

    map_array(objects( (: clonep :) ), (: counts[base_name($1)]++ :) );

    clone_object(MORE_OB)->more_string(sprintf("%O\n", counts));

    counts = 0;
    return 1;
}
