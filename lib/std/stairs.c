/* Do not remove the headers from this file! see /USAGE for more info. */

inherit EXIT_OBJ;

mixed direct_get_obj( object ob ) {
    if( query_plural())
        return "#Try climbing them instead.";
    else
        return "#Try climbing it instead.";
}

varargs protected void setup_messages(string name, mixed up_dest, mixed down_dest, mixed prep_only)
{
    string dirs;
    if (up_dest && (!stringp(up_dest) || strlen(up_dest) > 0)) {
        if (!arrayp(prep_only))
            add_exit("up", up_dest, prep_only);
        else
            add_exit("up", up_dest, prep_only[0]);
    }

    if (down_dest && (!stringp(down_dest) || strlen(down_dest) > 0)) {
        if (!arrayp(prep_only))
            add_exit("down", down_dest, prep_only);
        else
            add_exit("down", down_dest, prep_only[1]);
    }

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
    set_go_method("climb");
}


// Separated from create() so that objects can overload this separately
void more_create(mixed up_dest, mixed down_dest, int prep_only) {
    set_plural(1);
    setup_messages("stairs", up_dest, down_dest, prep_only);
}


void mudlib_setup( mixed up_dest, mixed down_dest, int prep_only )
{
    ::mudlib_setup();
    more_create( up_dest, down_dest, prep_only );
    set_attached( 1 );
}

