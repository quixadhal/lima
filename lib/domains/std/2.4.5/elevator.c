// Do not remove the headers from this file! see /USAGE for more info.
#include <hooks.h>

inherit MOVING_ROOM;

/* The destinations with their lamps lit */
array queue = ({});

void hook_elevator_door_closed();

void setup()
{
    set_brief("Elevator");
    set_long("You are in the elevator.  There are four buttons, labeled 'church', 'attic', 'wiz hall' and 'lima'.$elevator_door");

    set_state_description("elevator_door_on", "\nThere is an open door to the east.\n");
    set_state_description("elevator_door_off", "\nThere is a closed door to the east.\n");

    set_destinations(([
	"lima" : "/domains/std/Wizroom",
	"attic" : __DIR__ "attic.scr",
	"church" : __DIR__ "church.scr",
	"wiz hall" : __DIR__ "wiz_hall.scr",
      ]));

    move_to("church");
    set_distance("church", "wiz hall", 26);
    set_distance("attic", "wiz hall", 32);
    set_distance("lima", "church", 16);
    set_distance("lima", "attic", 10);
    set_distance("lima", "wiz hall", 42);
    set_in_progress((: tell_from_inside, this_object(), "The elevator continues ...\n" :), 5);

    new(__DIR__ "obj/elevator_button.scr", "church")->move(this_object());
    new(__DIR__ "obj/elevator_button.scr", "attic")->move(this_object());
    new(__DIR__ "obj/elevator_button.scr", "wiz hall")->move(this_object());
    new(__DIR__ "obj/elevator_button.scr", "lima")->move(this_object());

    set_objects( ([
	__DIR__ "obj/elevator_door" : ({ "east", "church" }),
      ]) );

    add_hook("elevator_door_closed", (: hook_elevator_door_closed :));
}

void move_to(string dest) {
    if (dest == "lima" || (dest == "attic" && query_where() != "lima") || (dest == "church" && query_where() == "wiz hall")) {
	tell_from_inside(this_object(), "The elevator jerks upward.\n");
    } else {
	tell_from_inside(this_object(), "The elevator starts moving downward.\n");
    }
    ::move_to(dest);    
}

void handle_queue() {
    if (query_in_motion()|| !sizeof(queue))
	return;

    call_hooks("elevator_will_move", HOOK_IGNORE, 0);
}

void hook_elevator_door_closed() {
    if (!sizeof(queue))
	return;

    move_to(queue[0]);
}

/* add a destination to the queue. */
void add_to_queue(string where) {
    if (member_array(where, queue) != -1)
	return;
    queue += ({ where });
    handle_queue();
}

void handle_press(string dest) {
    this_body()->simple_action("$N $vpress the '" + dest + "' button.\n");

    if (query_where() == dest)
	call_hooks("elevator_arrived", HOOK_IGNORE, 0, query_location());
    else 
	add_to_queue(dest);
}

void arrive() {
    tell_from_inside(this_object(), "The elevator slows down and stops\n");
    ::arrive();
    queue -= ({ query_where() });
    call_hooks("elevator_arrived", HOOK_IGNORE, 0, query_location());
}

int call_elevator(string where) {
    if (query_where() == where) {
	call_hooks("elevator_arrived", HOOK_IGNORE, 0, query_location());
	return 0;
    }

    if (member_array(where, queue) != -1) {
	write("Nothing happens.\n");
	return 0;
    }

    add_to_queue(where);

    return 1;
}
