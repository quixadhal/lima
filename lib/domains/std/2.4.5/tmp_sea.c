/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
mixed can_go_down() {
    // rather, um, strange, but compat.
    foreach (object ob in all_inventory(this_body())) {
       if (ob->container())
           return 1;
    }
    return "You can't breathe under water !\nYou should try to get some portable air supply!\n";
}

mixed swim() {
    mixed t;

    if (stringp(t = can_go_down())) {
 	write(t);
    } else {
        this_body()->simple_action("$N $vdive down into the sea.");
	this_body()->move_to("/domains/std/2.4.5/sea_bottom.scr", "down");
    }
    return 1;
}
void setup() {
function f;
set_exits( ([
  "west" : "jetty2.scr",
  "down" : "sea_bottom.scr",
]) );
set_brief("All at sea");
set_long("You are swimming out at the sea.");
}

