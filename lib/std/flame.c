/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void mudlib_setup()
{
    set_id("flame", "fire");
    set_long( "It's a pleasant little fire.");
}

mixed indirect_light_obj_with_obj()
{
    return 1;
}

