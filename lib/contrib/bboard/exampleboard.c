/* Do not remove the headers from this file! see /USAGE for more info. */


inherit "/open/hiccups/bboard";

void setup() {
	set_id("board");
	set_in_room_desc("A test board");
	set_board_name("test");
	init_board();	// This must be called after set_board_name().
}
