/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** v_plains.c -- example use of a virtual object server to create plains
**
** Format for the object is:
**
**	/domains/std/rooms/v_plains/x/y
**
** The data for the plains is pulled from v_plains.data.  It is an array
** of description-id values (in the first N non-comment lines).  Comments
** are lines beginning with #.  After the description array are lists of
** rooms for the boundaries of the plains.  They are in north, east,
** south, west order; one line per grid spot.
**
** Deathblade, 960101: created
*/

inherit GRID_SERVER;

#define DATA_FILE	__DIR__ "v_plains.data"

static private string * my_descriptions = ({
    "A dry, harsh wind blows over the gentle rolling hills of these plains.",
    "Wild grains and grasses push up through the hard earth.",
    "The clear blue sky heats the surrounding area, hardening the ground into baked clay.",
    "A small stream languidly flows along here.",
});

void create()
{
    ::create();

    use_data_file(DATA_FILE);
    set_brief("Plains");
    set_descriptions(my_descriptions);
}
