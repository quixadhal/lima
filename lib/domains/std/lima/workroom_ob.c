/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_OPENABLE;
inherit M_GETTABLE;

void setup() {
    set_id("workroom");

    set_long("Your workroom sits patiently in your home directory, waiting for you to enter it.  Although not well known by most of the LPmud community, the 'home' command is simply a slick user interface invented very early on in LPmud history.  Workrooms are actually entered by opening them.");
    set_closed(1);
    set_droppable("It doesn't seem to want to leave your directory.");
}

mixed direct_open_obj() {
    object env = this_object();

    while (env = environment(env)) {
	if (env == this_body())
	    return 1;
    }
    return "#You do not have that.\n";
}

void open_with() {
    write("As you start opening your workroom, a burst of static darts across your screen.  It wavers for a minute, then stablizes.  Several lines of hexadecimal scroll across the bottom.  You hear high pitched laughter in the background as the monitor dims and fuzzes out, then regains focus.  You look around, and notice you aren't quite where you thought you were going to end up.\n");
    this_body()->move_to(__DIR__ "beeks_workroom");
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_openable::lpscript_attributes() + m_gettable::lpscript_attributes();
}
