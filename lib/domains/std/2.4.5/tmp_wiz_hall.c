/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
mixed can_go_west();
mixed can_go_north() {
    if (!wizardp(this_body()))
	return "A magic force stops you.\n";
    return 1;
}

mixed do_go_north() {
  write("You wriggle through the force field...\n");
  return 0;
}

void arrived() {
    if (query_state("lamp"))
        tell_from_inside(this_object(), "The lamp on the button beside the elevator goes out.\n");
    clear_room_state("lamp");
}
void setup() {
function f;
set_brief("wizards hall");
set_long("You are in the hall of the wizards.\n There is a door to the west and a shimmering field to the north.\n");
set_state_description("lamp_on", "\nThere is a lit lamp beside the elevator.\n");
set_light(1);
set_objects( ([
  "obj/leo.scr" : 1,
  "obj/elevator_door.scr" : ({ "wiz hall" }),
  "obj/elevator_call_button.scr" : ({ "wiz hall" }),
]) );
set_exits( ([
  "west" : "elevator.scr",
  "north" : "quest_room.scr",
]) );
}


mixed can_go_west() {
{
if (!call_other("/domains/std/2.4.5/elevator.scr", "query_open_at","wiz hall"))
  return "The door is closed.\n";
return 1;;
}
;
}

