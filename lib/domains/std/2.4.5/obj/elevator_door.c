// Do not remove the headers from this file! see /USAGE for more info.
#include <hooks.h>

inherit DOOR;


mixed door_closing = 0;


void setup(string dir, string dest)
{
    set_id("door");
    set_adj("elevator");
    set_long("This is an elevator door.");
    set_attached(1);

    set_sibling_ident("elevator_door");
    set_door_direction(dir);
    set_door_destination(dest);
}

void call_out_close_door()
{
    door_closing = 0;

    do_on_close();
    set_closed(1);
    environment(this_object())->call_hooks("elevator_door_closed", HOOK_IGNORE, 0);
}

void hook_elevator_will_move()
{
    // Should never happen, really..
    if (!door_closing && !query_closed())
	door_closing = call_out("callout_close_door", 15);
    else environment(this_object())->call_hooks("elevator_door_closed", HOOK_IGNORE, 0);
}

void hook_elevator_arrived(mixed dest)
{
    set_door_destination(dest);
    setup_sibling(0, dest);
    do_on_open();
    set_closed(0);

    call_out("call_out_close_door", 15);
}

mixed direct_open_obj(object ob)
{
    mixed ret = ::direct_open_obj(ob);

    if (!ret || stringp(ret))
	return ret;

    return "Try pressing the elevator button instead.";
}

mixed direct_close_obj(object ob)
{
    mixed ret = ::direct_close_obj(ob);

    if (!ret || stringp(ret))
	return ret;

    return "Try pressing the elevator button instead.";
}

void do_on_open()
{
    ::do_on_open();
    environment(this_object())->set_room_state("elevator_door");
}

void do_on_close()
{
    ::do_on_close();
    environment(this_object())->clear_room_state("elevator_door");
}

void on_clone(mixed args...)
{
    object env = environment(this_object());

    ::on_clone(args...);
    if(is_open()) do_on_open();
    else do_on_close();

    env->add_hook("elevator_will_move", (: hook_elevator_will_move :));
    env->add_hook("elevator_arrived", (: hook_elevator_arrived :));
}
