/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_CLIMBABLE;


mixed direct_get_obj( object ob ) {
    if( query_plural())
	return "#Try climbing them instead.\n";
    else
	return "#Try climbing it instead.\n";
}

static void setup_messages(string name, mixed up_dest, mixed down_dest) {
    if (query_plural()) {
	add_id_no_plural(name);

	if (up_dest) {
	    if (down_dest) {
		set_long("The " + name + " lead up and down.\n");
		set_in_room_desc("There are " + name + " here, leading up and down.");
	    } else {
		set_long("The " + name + " lead upwards.\n");
		set_in_room_desc("There are " + name + " here, leading upwards.");
	    }
	} else {
	    set_long("The " + name + " lead downwards.\n");
	    set_in_room_desc("There are " + name + " here, leading downwards.");
	}
    } else {
	add_id(name);

	if (up_dest) {
	    if (down_dest) {
		set_long("The " + name + " leads up and down.\n");
		set_in_room_desc("There is a " + name + " here, leading up and down.");
	    } else {
		set_long("The " + name + " leads upwards.\n");
		set_in_room_desc("There is a " + name + " here, leading upwards.");
	    }
	} else {
	    set_long("The " + name + " leads downwards.\n");
	    set_in_room_desc("There is a " + name + " here, leading downwards.");
	}
    }
}

// Separated from create() so that objects can overload this separately
void more_create(mixed up_dest, mixed down_dest, int attached) {
    set_plural(1);
    setup_messages("stairs", up_dest, down_dest);
}

void create( mixed up_dest, mixed down_dest, int attached )
{
::create();
    more_create( up_dest, down_dest, attached );
    if( attached )
        set_flag( ATTACHED );
    set_up_destination(up_dest);
    set_down_destination(down_dest);
}
