/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_CLIMBABLE;

void create(mixed up_dest, mixed down_dest) {
    ::create();
    add_id("ladder");
    set_up_destination(up_dest);
    set_down_destination(down_dest);
    // Just a default.
    if (up_dest) {
	if (down_dest) {
	    set_long("The ladder leads up and down.\n");
	    set_in_room_desc("There is a ladder here, leading up and down.\n");
	} else {
	    set_long("The ladder leads upwards.\n");
	    set_in_room_desc("There is a ladder here, leading upwards.\n");
	}
    } else {
	set_long("The ladder leads downwards.\n");
	set_in_room_desc("There is a ladder here, leading downwards.\n");
    }
}
