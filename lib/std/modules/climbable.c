/* Do not remove the headers from this file! see /USAGE for more info. */

#include <hooks.h>

mixed up_dest, down_dest;

mixed direct_climb_obj() {
    if (up_dest && down_dest)
	return "Do you want to climb up it or down it?\n";
    if (up_dest || down_dest) return 1;
    return 0;
}

mixed direct_climb_up_obj() {
    if (up_dest) return 1;
    if (down_dest) return "Try climbing down it.\n";
    return 0;
}

mixed direct_climb_down_obj() {
    if (down_dest) return 1;
    if (up_dest) return "Try climbing up it.\n";
    return 0;
}

void set_up_destination(mixed x) {
    up_dest = x;
}

void set_down_destination(mixed x) {
    down_dest = x;
}

void use(mixed dest, mixed dir) {
    mixed ret;
    object where = environment(this_body());
    dest = evaluate(dest);
//### This code we call should be modularized, so we don't need this hack
    if (where->evaluate_destination(dest, dir))
        where->call_hooks("person_left", HOOK_IGNORE, 0, dir);
    if (where != environment(this_body()))
        force_look();
}

// we assume direction is "up", "down" or 0
void climb(string direction) {
    if (direction == "up" || (!direction && up_dest)) {
	use(up_dest, "up");
    } else {
	use(down_dest, "down");
    }
}
