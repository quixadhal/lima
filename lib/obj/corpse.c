/* Do not remove the headers from this file! see /USAGE for more info. */
inherit CONTAINER;

string the_name;

mixed ob_state() {
    return the_name;
}

void create(string name, string long)
{
    the_name = name;

    ::create(1);	/* skip the setup() call */
    if (!the_name) return;
    
    set_long(long || "The lifeless body of " + the_name + " isn't particularly interesting.");

    /* do the setup() call now that we're initialized */
    do_setup();
}

//### we need initial args passed to here...
void mudlib_setup()
{
    ::mudlib_setup();

    if ( !the_name )
	return;

    set_id("corpse", "body");
    if (the_name[<1] == 's') {
	set_adj("lifeless", the_name);
	set_proper_name(the_name + "' corpse");
    } else {
	set_adj("lifeless", the_name + "s");
	set_proper_name(the_name + "'s corpse");
    }
    set_in_room_desc("The lifeless body of " + the_name + " lies on the ground.");
    set_plural_in_room_desc("Lying on the ground are %s corpses of " + the_name + ".");
    set_max_capacity(VERY_LARGE);
    set_size(VERY_LARGE);
    set_preposition("on");
}

string introduce_contents() {
    return "On " + the_short() + " you see:\n";
}    
