/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
#undef EXTRA_LONG
#define EXTRA_LONG\
    if (id(str)) {\
	write("WARNING !!\n\n"+\
	    "The mines are closed due to risk of falling rock.\n");\
	return;\
    }

id(str) {
    return str == "sign" || str == "pole";
}
void setup() {
function f;
set_light(1);
set_brief("Mine entrance");
set_exits( ([
  "south" : "../mount_pass.scr",
  "north" : "tunnel2.scr",
]) );
}

