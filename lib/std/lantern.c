/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_SWITCHABLE;
inherit M_LIGHTABLE;


void turn_on()
{
    set_is_on( 1 );
    do_light();
}

void turn_off()
{
    set_is_on( 0 );
    do_extinguish();
}

mixed direct_flip_obj_on( object ob )
{
    return ::direct_light_obj( ob );
}

mixed direct_flip_obj_off( object ob )
{
    return ::direct_extinguish_obj( ob );
}


void mudlib_setup() {
    set_id("lantern");
    set_size(MEDIUM);
    set_long( "It's an unremarkable lantern.");

    set_fuel(3600);
 }
