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

private void main(mixed args)
{
    mapping counts = ([ ]);

    objects((: $(counts)[base_name($1)]++ :));

    /* filter out those without the requested minimum number of instances.
       defaults to 2 or more (meaning cloned obs) */
    if ( !args[0] )
	args[0] = 2;
    counts = filter(counts, (: $2 >= $(args[0]) :));

    out(sprintf("%O\n", counts));
}
