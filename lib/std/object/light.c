/* Do not remove the headers from this file! see /USAGE for more info. */

private int our_light;

//:FUNCTION our_room
//Return the room we are in, if we can be seen from there
static object our_room() {
    object env;
    object tmp;
    env = environment();
    if (!env) return 0;
    while (1) {
	// If we're inside something non-transparent, return 0
	if ((tmp = environment(env)) && !env->inventory_visible())
	    return 0;
	// if we're at the top of the tree, return the object we found
	if (!tmp) return env;
	env = tmp;
    }
}

//:FUNCTION set_light
//Set the amount of light that this object gives off
void set_light(int x) {
    object env;
    if (env = our_room()) {
	env->adjust_light(x-our_light);
    }
    our_light = x;
}

//:FUNCTION query_light
//Find out how much light the current object gives off
int query_light() { return our_light; }

