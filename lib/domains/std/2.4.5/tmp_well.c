/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
mixed can_go_west();
#undef EXTRA_LONG
#define EXTRA_LONG\
    if (str == "lever") {\
	write("The lever can be moved between two positions.\n");\
	return;\
    }\
    if (str == "door") {\
	if (call_other("room/sub/door_trap", "query_west_door"))\
	    write("The door is closed.\n");\
	else\
	    write("The door is open\n");\
    }

close(str) {
    if (!str && str != "door")
	return 0;
    write("You can't.\n");
    return 1;
}

open(str) {
    if (!str && str != "door")
	return 0;
    write("You can't.\n");
    return 1;
}

pull(str) {
    if (!str || str != "lever")
	return 0;
    call_other("room/sub/door_trap", "toggle_door");
    return 1;
}

id(str) {
    return str == "lever" || str == "door";
}
void setup() {
function f;
set_exits( ([
  "up" : "narr_alley.scr",
  "north" : "maze1/maze1.scr",
  "west" : "sub/door_trap.scr",
]) );
set_brief("Down the well");
set_long("You are down the well.  It is wet and slippery.\n There is a lever beside a door to the west.");
}


mixed can_go_west() {
{
if (call_other("/domains/std/2.4.5/sub/door_trap.scr", "query_west_door"))
  return "The door is closed.\n";
return 1;;
}
;
}

