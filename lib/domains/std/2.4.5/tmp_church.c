/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
mixed can_go_west();
void arrived() {
    if (query_state("lamp"))
        tell_from_inside(this_object(), "The lamp on the button beside the elevator goes out.\n");
    clear_room_state("lamp");
}
void setup() {
function f;
set_light(1);
set_brief("Village church");
set_objects( ([
  "/domains/std/2.4.5/tmp_church_0.scr" : 1,
  "obj/elevator_door.scr" : ({ "church" }),
  "obj/elevator_call_button.scr" : ({ "church" }),
]) );
add_item("pit", "In the middle of the church is a deep pit.\n It was used for sacrifice in the old times, but nowadays\n it is only left for tourists to look at.\n");
set_long("You are in the local village church.\nThere is a huge pit in the center,\n and a door in the west wall. There is a button beside the door.\n This church has the service of reviving ghosts. Dead people come\n to the church and pray.\n There is a clock on the wall.\n There is an exit to south.\n");
set_state_description("lamp_on", "\nThe lamp beside the elevator is lit.\n");
set_exits( ([
  "west" : "elevator.scr",
  "south" : "vill_green.scr",
]) );
set_objects( ([
  "obj/elevator_door.scr" : ({ "church" }),
  "obj/elevator_call_button.scr" : ({ "church" }),
]) );
}


mixed can_go_west() {
{
if (!call_other("/domains/std/2.4.5/elevator.scr", "query_open_at","church"))
  return "The door is closed.\n";
return 1;;
}
;
}

