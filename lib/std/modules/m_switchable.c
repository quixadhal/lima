/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <hooks.h>

private int 		is_on;
private string		switch_on_msg = "$N $vswitch a $o on.";
private string		switch_off_msg = "$N $vswitch a $o off.";

void hook_state(string, string, int);
void add_hook(string, mixed);
mixed call_hooks(string, int);

void create() {
    // make abstract version work
    if (file_name() + ".c" == __FILE__) return;
    add_hook("direct_flip", "Flip it on or off?\n");
}

int query_is_on() {
    return is_on;
}

mixed set_is_on(int x) {
    string tmp;

    if (x) {
	if (is_on) return 0;
//:HOOK prevent_turn_on
//A yes/no/error hook that can prevent an object from being turned on.
	tmp = call_hooks("prevent_turn_on", HOOK_YES_NO_ERROR);
	if (!tmp || stringp(tmp)) return tmp;
	is_on = 1;
//:HOOK turn_on
//called after an object is turned on.  The return values are ignored.
	call_hooks("turn_on", HOOK_IGNORE);
    } else {
	if (!is_on) return 0;
//:HOOK prevent_turn_off
//A yes/no/error hook that can prevent an object from being turned off.
	tmp = call_hooks("prevent_turn_off", HOOK_YES_NO_ERROR);
	if (!tmp || stringp(tmp)) return tmp;
	is_on = 0;
//:HOOK turn_off
//called after an object is turned off.  The return values are ignored.
	call_hooks("turn_off", HOOK_IGNORE);
    }
    return 1;
}

void set_switch_on_msg(string x) {
    switch_on_msg = x;
}

void set_switch_off_msg(string x) {
    switch_off_msg = x;
}

// Verb callbacks

void turn_on() {
    string tmp = set_is_on(1);
    if (!tmp) tmp = "You can't seem to turn it on.\n";
    if (stringp(tmp)) write(tmp);
    else this_body()->simple_action(switch_on_msg, this_object());
}

void turn_off() {
    string tmp = set_is_on(0);
    if (!tmp) tmp = "You can't seem to turn it off.\n";
    if (stringp(tmp)) write(tmp);
    else this_body()->simple_action(switch_off_msg, this_object());
}

// Verb interaction

mixed direct_flip_off_obj(object ob) {
    if (!is_on) return "It isn't turned on!\n";
    return 1;
}

mixed direct_flip_on_obj(object ob) {
    if (is_on) return "It is already on!\n";
    return 1;
}

mixed direct_flip_obj_on(object ob) {
    if (is_on) return "It is already on!\n";
    return 1;
}

mixed direct_flip_obj_off(object ob) {
    if (!is_on) return "It isn't turned on!\n";
    return 1;
}
