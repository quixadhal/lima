/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void setup() {
    set_id("button");
    set_long( "It probably opens up the wall that you came through.");
    set_attached();
}

mixed direct_press_obj( object obj )
{
    return 1;
}

void do_press()
{
    object wall = present( "wall", environment( this_object()));

    if( wall && wall->query_closed())
    {
	this_body()->simple_action( "$N $vpress the button.");
	wall->trigger();
    }
    else
	this_body()->simple_action( "$N $vpress the button, but nothing happes." );
}
