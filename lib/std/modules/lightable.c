/* Do not remove the headers from this file! see /USAGE for more info. */

#include <hooks.h>

private int		is_lit;
private int		fuel;
private string		burned_out_msg = "The $o is burned out.\n";
private string		die_msg = "The light from the $o flickers and dies.\n";

mixed call_hooks(string, int);
void add_hook(string, mixed);
void remove_hook(string, mixed);

string the_short();

int query_is_lit() {
    return is_lit;
}

void set_burned_out_msg(string x) {
    burned_out_msg = x;
}

void set_die_msg(string x) {
    die_msg = x;
}

void set_fuel(int x) {
    fuel = x;
}

mixed extinguish() {
    string tmp;
    
    if (!is_lit) return 0;
//:HOOK prevent_extinguish
//A yes/no/error hook that can prevent an object from being extinguished
    tmp = call_hooks("prevent_extinguish", HOOK_YES_NO_ERROR);
    if (!tmp || stringp(tmp)) return tmp;
    is_lit = 0;
    if (fuel != -1) fuel = remove_call_out("burn_out");
//:HOOK extinguish
//called when an object is extinguished.  The return value is ignored
    call_hooks("extinguish", HOOK_IGNORE);
    remove_hook("extra_short", "providing light");
    return 1;
}

mixed check_fuel() {
    if (!fuel) return object_event_message(burned_out_msg);
    return 1;
}

mixed light() {
    string tmp;
    if (is_lit) return 0;

    if (!fuel) return object_event_message(burned_out_msg);
    
//:HOOK prevent_light
//A yes/no/error hook that can prevent an object from getting lit
    tmp = call_hooks("prevent_light", HOOK_YES_NO_ERROR);
    if (!tmp || stringp(tmp)) return tmp;
    is_lit = 1;
    if (fuel != -1) call_out("burn_out", fuel);
//:HOOK light
//called when an object is lit.  The return value is ignored.
    call_hooks("light", HOOK_IGNORE);
    add_hook("extra_short", "providing light");
    return 1;
}

void burn_out() {
    fuel = 0;
    is_lit = 0;
    call_hooks("extinguish", HOOK_IGNORE);
    object_event(die_msg);
}
