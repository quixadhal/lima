#pragma no_warnings
inherit "/std/indoor_room";

void setup();
void arrived() {
    if (query_state("lamp"))
        tell_from_inside(this_object(), "The lamp on the button beside the elevator goes out.\n");
    clear_room_state("lamp");
}
void setup() {
function f;
set_light(1);
set_brief("Village Church");
set_long("You are in the local village church. There is a huge pit in the c"
"enter, and an elevator door in the west wall. Next to the door, t"
"here's a button.\nThis church has the service of reviving ghosts."
" Dead people come to the church and pray.\nThere is a clock on th"
"e wall.\nThere is an exit to south.$lamp"
);
set_state_description("lamp_on", "\nThe lamp beside the elevator is lit.\n");
set_objects( ([
  "obj/elevator_door" : ({ "west", "/domains/std/2.4.5/elevator" }),
  "obj/elevator_call_button.scr" : ({ "church" }),
  "/domains/std/2.4.5/tmp_church_0.scr" : 1,
]) );
add_item("pit", "In the middle of the church is a deep pit.\n It was used for sacrifice in the old times, but nowadays\n it is only left for tourists to look at.\n");
set_exits( ([
  "south" : "vill_green.scr",
]) );
}

