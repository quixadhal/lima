/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;

void setup() {
    set_brief("Inside the White House");
    set_long("Wow!  You're actually inside the White House!  Maybe you'll get to see the president!  Oops, wrong white house, I guess.  But one can always hope.  It appears the house has been completely refurnished since you were last here.");
    set_light(1);
    set_objects( ([
	"rust" : 1
    ]) );
    set_exits( ([
	"west" : "entrance"
    ]) );
    set_default_exit("You walk into one of the walls, making a complete fool of yourself.\n");
}
