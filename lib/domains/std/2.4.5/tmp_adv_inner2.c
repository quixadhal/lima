/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
#undef EXTRA_RESET
#define EXTRA_RESET\
    if (!arg) {\
	 move_object(clone_object("obj/wiz_bull_board2"), this_object()); \
    }
void setup() {
function f;
set_exits( ([
  "north" : "adv_inner.scr",
]) );
set_brief("The LPC board");
set_long("This is the LPC discussion room.\n Only wizards can access this room.\n");
set_light(1);
}

