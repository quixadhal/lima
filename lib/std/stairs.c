/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_CLIMBABLE;


mixed direct_get_obj( object ob ) {
    if( query_plural())
	return "#Try climbing them instead.";
    else
	return "#Try climbing it instead.";
}

static void setup_messages(string name, mixed up_dest, mixed down_dest)
{
    string dirs;
    if (up_dest)
    {
	if (down_dest)
	    dirs = "up and down";
	else
	    dirs = "upwards";
    }
    else
    {
	if( down_dest )
	    dirs = "downwards";
	else
	    dirs = "nowhere";
    }

    if (query_plural())
	add_id_no_plural(name);
    else
	add_id( name );

    set_long( "The " +name+ " lead" +(query_plural()?" ":"s ") +dirs );
    set_in_room_desc( "There " +(query_plural()?"are ":"is a ")+name+ " here, leading " +dirs+ ".");
}


// Separated from create() so that objects can overload this separately
void more_create(mixed up_dest, mixed down_dest, int attached) {
    set_plural(1);
    setup_messages("stairs", up_dest, down_dest);
}


void mudlib_setup( mixed up_dest, mixed down_dest, int attached )
{
    ::mudlib_setup();
    more_create( up_dest, down_dest, attached );
    set_attached( attached );
}

void on_clone( mixed up_dest, mixed down_dest, int attached )
{
    ::on_clone( up_dest, down_dest, attached );
    if( up_dest )
	set_up_destination( absolute_path( up_dest, environment( )));
    if( down_dest )
	set_down_destination( absolute_path( down_dest, environment( )));
}
