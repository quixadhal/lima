/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_GETTABLE;

void setup() {
    set_adj("home");
    set_id("directory");
    
    set_long("This is the directory where you put all your files.");
    set_droppable("You appear to be stuck with it.");
    set_relations("in");
    set_default_relation("in");
    set_max_capacity(100);
    set_objects( ([ "workroom_ob" : 1 ]) );
}

mixed receive_object(object target, string relation) {
    if (relation != "#CLONE#")
	return "You think about putting that in your home directory, but decide against it.\n";
    return ::receive_object(target, relation);
}


mapping lpscript_attributes()
{
    return container::lpscript_attributes() + m_gettable::lpscript_attributes();
}
