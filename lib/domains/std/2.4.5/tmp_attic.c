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
set_brief("The attic");
set_objects( ([
  "obj/elevator_door.scr" : ({ "attic" }),
  "obj/elevator_call_button.scr" : ({ "attic" }),
]) );
set_exits( ([
  "west" : "elevator.scr",
]) );
set_long("This is the attic above the church.\n There is a door to the west.");
set_state_description("lamp_on", "\nThe lamp beside the elevator is lit.\n");
}


mixed can_go_west() {
{
if (!call_other("/domains/std/2.4.5/elevator.scr", "query_open_at","attic"))
  return "The door is closed.\n";
return 1;;
}
;
}

