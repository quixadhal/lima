/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_CLIMBABLE;

void create(mixed up_dest, mixed down_dest) {
    ::create();
    add_id("stairs");
    set_up_destination(up_dest);
    set_down_destination(down_dest);
    // Just a default.
    if (up_dest) {
	if (down_dest) {
	    set_long("The stairs lead up and down.\n");
	    set_in_room_desc("There are stairs here, leading up and down.\n");
	} else {
	    set_long("The stairs lead upwards.\n");
	    set_in_room_desc("There are stairs here, leading upwards.\n");
	}
    } else {
	set_long("The stairs lead downwards.\n");
	set_in_room_desc("There are stairs here, leading downwards.\n");
    }
}
