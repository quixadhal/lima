/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** base_obj.c -- base for the object classes
**
** This is an object with the very most basic features.  It shouldn't be
** used directly.  Use OBJ, SPARSE_OBJ, or LESS_SPARSE_OBJ instead.
**
** 960121, Deathblade: created
*/

inherit M_GRAMMAR;
inherit __DIR__ "object/names";
inherit __DIR__ "object/description";
inherit __DIR__ "object/flags";
inherit __DIR__ "object/non_object";


void create()
{
    parse_init();
    names::create();
    flags::create();
}

//:FUNCTION stat_me
//write() some debugging info about the state of the object
int stat_me() 
{
    write("Short: "+short()+"\n");
    write("IDs: "+implode(parse_command_id_list(),", ")+"\n");
    write("Plurals: "+implode(parse_command_plural_id_list(),", ")+"\n");
    write("Adjectives: "+implode(parse_command_adjectiv_id_list(),", ")+"\n");
    write("Long: \n"+long());
    return 1;
}

/*
** Add some very basic functionality.  Heavier weight objects will most
** likely override these and supply "real" functionality.
*/

int is_visible()
{
    return 1;
}

// Here on purpose.
varargs mixed call_hooks(mixed s ...)
{
}

/* need a set_light() for remove() */
void set_light(int x)
{
}
