/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
/* FIXME */
init(arg)
{
	object	ob;
	int	does_exist;

	if(this_player()) {
		if(
			!present("tech_quicktyper", this_player())
			&&
			!present("tech_quicktyper", this_object())
		) {
			/* it does no exist */
			ob = clone_object("obj/quicktyper");
			move_object(ob, this_object());
		}
	}
	::init(arg);
}

void setup() {
function f;
set_light(1);
set_brief("A small storage room");
set_long("You are in a small and dusty storage room.\n You can see the shop through the opening to the east.");
set_exits( ([
  "east" : "shop.scr",
]) );
}

