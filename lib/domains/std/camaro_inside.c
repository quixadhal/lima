/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** camaro_inside.c
**
** The inside of the camaro.  Conceivably, you could fit a lot of people
** in here, but we won't worry about that just yet.
**
** 951123, Deathblade: Created
*/

inherit INSIDE;

void create(object the_outside)
{
    ::create(the_outside);

    set_brief("Camaro");
    set_long("You're sitting inside the Camaro Z28.");
    set_light(1);
}
