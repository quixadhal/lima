/* Do not remove the headers from this file! see /USAGE for more info. */

#include <hooks.h>

private mixed		source_filter;
private int		is_lit;
private int		fuel;
private string		burned_out_msg = "The $o is burned out.\n";
private string		die_msg = "The light from the $o flickers and dies.\n";
private string		light_msg = "$N $vlight a $o.\n";
private string		light_with_msg = "$N $vlight a $o with a $o1.\n";
private string		extinguish_msg = "$N $vextinguish a $o.\n";

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

void set_extinguish_msg(string x) {
    extinguish_msg = x;
}

void set_light_msgs(string x, string y) {
    light_msg = x;
    light_with_msg = y;
}

void set_die_msg(string x) {
    die_msg = x;
}

void set_fuel(int x) {
    fuel = x;
}

//:FUNC set_source
//set_source(function f) makes it so that the object can only be lit by
//objects for which evaluate(f, ob) returns 1.  f may return a string
//error message on failure.  If f is a string, any object with that string
//as an id is a valid source.  set_source(0) makes any object a valid source.
void set_source(mixed f) {
    source_filter = f;
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

void do_extinguish() {
    mixed tmp = extinguish();
    if (!tmp) tmp = "That doesn't seem possible.\n";
    if (stringp(tmp)) {
	write(tmp);
	return;
    }
    this_body()->simple_action(extinguish_msg, this_object());
}

mixed check_fuel() {
    if (!fuel) return object_event_message(burned_out_msg);
    return 1;
}

varargs mixed light(object with) {
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

void do_light(object with) {
    mixed tmp = light(with);
    if (!tmp) tmp = "That doesn't seem possible.\n";
    if (stringp(tmp)) {
	write(tmp);
	return;
    }
    if (with)
	this_body()->simple_action(light_with_msg, this_object(), with);
    else
	this_body()->simple_action(light_msg, this_object());
}

void burn_out() {
    fuel = 0;
    is_lit = 0;
    call_hooks("extinguish", HOOK_IGNORE);
    object_event(die_msg);
}

mixed direct_light_obj() {
    if (source_filter)
	return "You need to light it with something.\n";
    return 1;
}

mixed direct_extinguish_obj() {
    if (!is_lit) return "It isn't lit.\n";
    return 1;
}

mixed direct_light_obj_with_obj(object ob, object with) {
    if (!source_filter) return 1;
    if (!with) return 1;
    if (stringp(source_filter)) return with->id(source_filter);
    return evaluate(source_filter, with);
}

mixed indirect_light_obj_with_obj(object ob, object with) {
    if (is_lit) return 1;
    return capitalize(with->the_short()) + " isn't lit.\n";
}
    
